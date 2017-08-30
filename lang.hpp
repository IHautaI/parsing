#ifndef LANG_HPP
#define LANG_HPP

#include <vector>
#include <memory>
#include <iostream>

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

class Node;
class node_expr;


class Node
{
public:
  int type;

  virtual auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<Node> = 0;
  virtual auto null() -> bool = 0;
  virtual auto cull(std::shared_ptr<Node>& x) -> bool = 0;

  Node(int i)
  : type(i)
  {}
};

template<typename T, typename... Args>
auto make_node(Args&&... args) -> std::shared_ptr<Node>;


class token_t : public Node
{
  bool nullable;
public:
  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<Node>
  {
    if( type == 0 || type == 1 || type != i.first )
    {
      return make_node<token_t>(0);
    }

    return make_node<token_t>(1);
  }

  auto null() -> bool {
    return nullable;
  }

  auto cull(std::shared_ptr<Node>& x) -> bool
  {
    if( type == 0 )
    {
      return true;
    }
    return false;
  }


  token_t(const int i)
  : Node(i)
  , nullable(i == 1)
  {}
};


class node_expr : public Node
{
  std::shared_ptr<Node> left;
  std::shared_ptr<Node> obj;
  std::pair<int, std::string> wrt;
  bool resolved;
  bool culled;

public:
  auto eval() -> void
  {
    obj = left->deriv(wrt);
  }

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<Node>
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

  auto cull(std::shared_ptr<Node>& x) -> bool
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


  node_expr(const std::shared_ptr<Node>& left, const std::pair<int, std::string>& i)
  : Node(-5)
  , left(left)
  , wrt(i)
  , resolved(false)
  , culled(false)
  {}
};


class nonterm_t : public Node {
  bool nullable;
  bool done;
  bool culled;

  std::pair<std::pair<int, std::string>, std::shared_ptr<Node>> last;

public:
  std::shared_ptr<Node> left;

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<Node>
  {
    if( last.first.first != i.first ){
      last = std::make_pair(i, make_node<nonterm_t>(make_node<node_expr>(left,
                                                                         i)));
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


  auto cull(std::shared_ptr<Node>& x) -> bool
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
  , nullable(false)
  , done(false)
  , culled(false)
  {}

  nonterm_t(const std::shared_ptr<Node>& l)
  : Node(-4)
  , left(l)
  , nullable(false)
  , done(false)
  , culled(false)
  {}
};


class or_t : public Node
{
  std::shared_ptr<Node> left;
  std::shared_ptr<Node> right;
public:
  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<Node>
  {
    return make_node<or_t>(make_node<node_expr>(left, i),
                           make_node<node_expr>(right, i));
  }

  auto null() -> bool {
    return left->null() || right->null();
  }

  auto cull(std::shared_ptr<Node>& x) -> bool
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


  or_t(const std::shared_ptr<Node>& l,
       const std::shared_ptr<Node>& r)
  : Node(-2)
  , left(l)
  , right(r)
  {}
};


class cat_t : public Node
{
  std::shared_ptr<Node> left;
  std::shared_ptr<Node> right;
public:
  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<Node>
  {
    if( left->null() )
    {
      return make_node<or_t>(
                make_node<cat_t>(make_node<node_expr>(left, i), right),
                make_node<node_expr>(right, i));
    }

    return make_node<cat_t>(make_node<node_expr>(left, i), right);
  }

  auto null() -> bool
  {
    return left->null() && right->null();
  }

  auto cull(std::shared_ptr<Node>& x) -> bool
  {
    if( left->cull(left) || right->cull(right) )
    {
      x.reset();
      return true;
    }
    return false;
  }


  cat_t(const std::shared_ptr<Node>& l,
        const std::shared_ptr<Node>& r)
  : Node(-3)
  , left(l)
  , right(r)
  {}
};


class star_t : public Node {
  std::shared_ptr<Node> left;
public:
  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<Node>
  {
    return make_node<cat_t>(make_node<node_expr>(left, i),
                            std::shared_ptr<Node>(
                              dynamic_cast<Node*>(this)));
  }

  auto null() -> bool {
    return true;
  }

  auto cull(std::shared_ptr<Node>& x) -> bool
  {
    if( left->type != 0 )
    {
      if( left->cull(left) )
      {
        left = make_node<token_t>(0);
      }
    }

    return false;
  }


  star_t(const std::shared_ptr<Node>& n)
  : Node(-1)
  , left(n)
  {}
};


template<typename T, typename... Args>
auto make_node(Args&&... args) -> std::shared_ptr<Node>
{
  return std::shared_ptr<Node>(dynamic_cast<Node*>(new T(args...)));
}


template<typename T, typename Arg, typename Args2>
auto make_nodes(Arg&& arg, Args2&& arg2) -> std::shared_ptr<Node>
{
  return std::shared_ptr<Node>(dynamic_cast<Node*>(new T(arg, arg2)));
}

template<typename T, typename Arg, typename... Args>
auto make_nodes(Arg&& arg, Args&&... args) -> std::shared_ptr<Node>
{
  return std::shared_ptr<Node>(dynamic_cast<Node*>(new T(arg, make_nodes<T>(args...))));
}

}
#endif
