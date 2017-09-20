#ifndef LANG_HPP
#define LANG_HPP

#include <vector>
#include <iostream>

#include "node_t.hpp"

namespace lang {
/*
*  types are integers
*  -1 = or, -2 = and, -3 = star,
*  0 = null, 1 = epsilon, leaves
*  type < -3 : unused
*  type > 1  : user/language-defined
*  nonterm_t : Language or other nonterminal,
*   user-defined
*/


// syntax tree
// class S_node
// {
// public:
//   std::string value;
//   std::vector<std::unique_ptr<S_node>> children;
//
//   friend std::ostream& operator<<(std::ostream& out, S_node& s)
//   {
//     out << s.value;
//     for(auto& x : s.children)
//     {
//       out << *x;
//     }
//
//     return out;
//   }
// };



template<typename T, typename... Args>
auto make_node(Args&&... args) -> std::shared_ptr<node_t>;


class token_t : public Node<token_t>
{
public:

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>
  {
    if( type() == 0 || type() == 1 || type() != i.first )
    {
      return make_node<token_t>(0);
    }

    return make_node<token_t>(1);
  }

  auto null() -> bool
  {
    return nullable;
  }

  auto cull(std::shared_ptr<node_t>& x) -> bool
  {
    if( type() == 0 )
    {
      return true;
    }
    return false;
  }

  token_t(const int i)
  : Node(i, i == 1)
  {}

  ~token_t(){}
};


class node_expr : public Node<node_expr>
{
  std::shared_ptr<node_t> left;
  std::shared_ptr<node_t> obj;
  std::pair<int, std::string> wrt;
  bool resolved;
  bool culled;

public:

  auto eval() -> void
  {
    obj = left->deriv(wrt);
  }

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>
  {
    if( !resolved )
    {
      eval();
    }
    return obj->deriv(i);
  }

  auto null() -> bool
  {
    if( !resolved )
    {
      eval();
      resolved = true;
    }

    return obj->null();
  }

  auto cull(std::shared_ptr<node_t>& x) -> bool
  {
    if( resolved && !culled )
    {
      culled = true;

      if( obj->cull(obj) )
      {
        x.reset();
        return true;
      }

      x = obj;
      return false;
    }

    return false;
  }

  node_expr(const std::shared_ptr<node_t>& left, const std::pair<int, std::string>& i)
  : Node(-5)
  , left(left)
  , wrt(i)
  , resolved(false)
  , culled(false)
  {}

  ~node_expr(){}
};


class nonterm_t : public Node<nonterm_t>
{
  bool done;
  bool culled;

  std::pair<std::pair<int, std::string>, std::shared_ptr<node_t>> last;

public:

  std::shared_ptr<node_t> left;

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>
  {
    if( last.first.first != i.first )
    {
      last = std::make_pair(i,
                            make_node<nonterm_t>(make_node<node_expr>(left, i)));
    }

    return last.second;
  }

  auto null() -> bool
  {
    if( !done )
    { // uses false as default
      // when finding itself in it's contained nodes
      done = true;
      nullable = left->null();
    }

    return nullable;
  }

  auto cull(std::shared_ptr<node_t>& x) -> bool
  {
    if( ! culled )
    {
      culled = true;

      if( left->cull(left) )
      {
        x.reset();
        return true;
      }

      culled = false;
    }

    return false;
  }

  nonterm_t()
  : Node(-4)
  , done(false)
  , culled(false)
  {}

  nonterm_t(const std::shared_ptr<node_t>& l)
  : Node(-4)
  , left(l)
  , done(false)
  , culled(false)
  {}

  ~nonterm_t(){}
};


class or_t : public Node<or_t>
{
  std::shared_ptr<node_t> left;
  std::shared_ptr<node_t> right;

public:

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>
  {
    return make_node<or_t>(make_node<node_expr>(left, i),
                           make_node<node_expr>(right, i));
  }

  auto null() -> bool {
    return left->null() || right->null();
  }

  auto cull(std::shared_ptr<node_t>& x) -> bool
  {
    auto l = left->cull(left);
    auto r = right->cull(right);

    if( l || r )
    {
      if( l && r )
      {
        x.reset();
        return true;
      } else if( l )
      {
        x = right;
      } else if( r )
      {
        x = left;
      }
    }

    return false;
  }

  or_t(const std::shared_ptr<node_t>& l,
       const std::shared_ptr<node_t>& r)
  : Node(-2)
  , left(l)
  , right(r)
  {}

  ~or_t(){};
};


class cat_t : public Node<cat_t>
{
  std::shared_ptr<node_t> left;
  std::shared_ptr<node_t> right;

public:

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>
  {
    auto a = make_node<cat_t>(make_node<node_expr>(left, i), right);

    if( left->null() )
    {
      return make_node<or_t>(a, make_node<node_expr>(right, i));
    }

    return a;
  }

  auto null() -> bool
  {
    return left->null() && right->null();
  }

  auto cull(std::shared_ptr<node_t>& x) -> bool
  {
    if( left->cull(left) || right->cull(right) )
    {
      x.reset();
      return true;
    }
    return false;
  }

  cat_t(const std::shared_ptr<node_t>& l,
        const std::shared_ptr<node_t>& r)
  : Node(-3)
  , left(l)
  , right(r)
  {}

  ~cat_t(){}
};


class star_t : public Node<star_t>
{
  std::shared_ptr<node_t> left;

public:

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>
  {
    return make_node<cat_t>(make_node<node_expr>(left, i),
                            std::make_shared<node_t>(this, tag_t::Star));
  }

  auto null() -> bool
  {
    return true;
  }

  auto cull(std::shared_ptr<node_t>& x) -> bool
  {
    if( left->type() != 0 )
    {
      if( left->cull(left) )
      {
        left = make_node<token_t>(0);
      }
    }

    return false;
  }

  star_t(const std::shared_ptr<node_t>& n)
  : Node(-1, true)
  , left(n)
  {}

  ~star_t(){}
};


template<typename T, typename... Args>
auto make_node(Args&&... args) -> std::shared_ptr<node_t>
{
  return std::make_shared<node_t>(new T(args...), get_tag<T>().tag);
}


template<typename T, typename Arg, typename Args2>
auto make_nodes(Arg&& arg, Args2&& arg2) -> std::shared_ptr<node_t>
{
  return std::make_shared<node_t>(new T(arg, arg2), get_tag<T>().tag);
}

template<typename T, typename Arg, typename... Args>
auto make_nodes(Arg&& arg, Args&&... args) -> std::shared_ptr<node_t>
{
  return std::make_shared<node_t>(new T(arg, make_nodes<T>(args...)),
                                  get_tag<T>().tag);
}

}
#endif
