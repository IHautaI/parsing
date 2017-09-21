/*
*  Defines a generic node_t type
*  to contain nodes via unique_ptr<void>
*  with a custom deleter deleter_t
*  and type-tags (an enum)
*/

#ifndef NODE_T_HPP
#define NODE_T_HPP


#include <utility>
#include <functional>
#include <memory>

// #include "lang.hpp"

namespace lang {

// template<typename T>
// class Node;
//

class node_t;

/*
*  main node interface
*  CRTP class
*/
template<typename T>
class Node
{
  auto mine() -> T&
  {
    return *static_cast<T*>(this);
  }

public:

  int _type;
  bool nullable;

  auto type() -> int
  {
    return _type;
  }

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>
  {
    return this->mine().deriv(i);
  }

  auto null() -> bool
  {
    return this->mine().null();
  }

  auto cull(std::shared_ptr<node_t>& x) -> bool
  {
    return this->mine().cull(x);
  }

  Node(int i, bool nullable = false)
  : _type(i)
  , nullable(nullable)
  {}

  ~Node(){}
};


class node_expr;
class token_t;
class or_t;
class cat_t;
class star_t;
class nonterm_t;

enum class tag_t
{
  Expr,
  Token,
  And,
  Or,
  Star,
  NonTerm
};


class deleter_t
{
  tag_t tag;
public:
  auto operator()(void* x) -> void
  {
    switch(tag)
    {
      case tag_t::Expr:
        delete static_cast<node_expr*>(x);
        break;
      case tag_t::Token:
        delete static_cast<token_t*>(x);
        break;
      case tag_t::And:
        delete static_cast<cat_t*>(x);
        break;
      case tag_t::Or:
        delete static_cast<or_t*>(x);
        break;
      case tag_t::Star:
        delete static_cast<star_t*>(x);
        break;
      case tag_t::NonTerm:
        delete static_cast<nonterm_t*>(x);
        break;
    }
  }

  deleter_t(tag_t t)
  : tag(t)
  {}

  ~deleter_t(){};
};


class node_t
{
  std::unique_ptr<void, std::function<void(void*)>> node;
  tag_t tag;



public:
  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>
  {
    switch(tag)
    {
      case tag_t::Expr:
        return static_cast<Node<node_expr>*>(node.get())->deriv(i);
        break;
      case tag_t::Token:
        return static_cast<Node<token_t>*>(node.get())->deriv(i);
        break;
      case tag_t::And:
        return static_cast<Node<cat_t>*>(node.get())->deriv(i);
        break;
      case tag_t::Or:
        return static_cast<Node<or_t>*>(node.get())->deriv(i);
        break;
      case tag_t::Star:
        return static_cast<Node<star_t>*>(node.get())->deriv(i);
        break;
      case tag_t::NonTerm:
        return static_cast<Node<nonterm_t>*>(node.get())->deriv(i);
        break;
    }
  }

  auto null() -> bool
  {
    switch(tag)
    {
      case tag_t::Expr:
        return static_cast<Node<node_expr>*>(node.get())->null();
        break;
      case tag_t::Token:
        return static_cast<Node<token_t>*>(node.get())->null();
        break;
      case tag_t::And:
        return static_cast<Node<cat_t>*>(node.get())->null();
        break;
      case tag_t::Or:
        return static_cast<Node<or_t>*>(node.get())->null();
        break;
      case tag_t::Star:
        return static_cast<Node<star_t>*>(node.get())->null();
        break;
      case tag_t::NonTerm:
        return static_cast<Node<nonterm_t>*>(node.get())->null();
        break;
    }
  }

  auto cull(std::shared_ptr<node_t>& x) -> bool
  {
    switch(tag)
    {
      case tag_t::Expr:
        return static_cast<Node<node_expr>*>(node.get())->cull(x);
        break;
      case tag_t::Token:
        return static_cast<Node<token_t>*>(node.get())->cull(x);
        break;
      case tag_t::And:
        return static_cast<Node<cat_t>*>(node.get())->cull(x);
        break;
      case tag_t::Or:
        return static_cast<Node<or_t>*>(node.get())->cull(x);
        break;
      case tag_t::Star:
        return static_cast<Node<star_t>*>(node.get())->cull(x);
        break;
      case tag_t::NonTerm:
        return static_cast<Node<nonterm_t>*>(node.get())->cull(x);
        break;
    }
  }

  auto type() -> int
  {
    switch(tag)
    {
      case tag_t::Expr:
        return static_cast<Node<node_expr>*>(node.get())->type();
        break;
      case tag_t::Token:
        return static_cast<Node<token_t>*>(node.get())->type();
        break;
      case tag_t::And:
        return static_cast<Node<cat_t>*>(node.get())->type();
        break;
      case tag_t::Or:
        return static_cast<Node<or_t>*>(node.get())->type();
        break;
      case tag_t::Star:
        return static_cast<Node<star_t>*>(node.get())->type();
        break;
      case tag_t::NonTerm:
        return static_cast<Node<nonterm_t>*>(node.get())->type();
        break;
    }
  }

  template<typename T>
  auto as() -> T&
  {
    return *static_cast<T*>(node.get());
  }

  template<typename T>
  node_t(T* x, tag_t t)
  : node(static_cast<void*>(x), deleter_t(t))
  , tag(t)
  {}

  ~node_t(){}
};


template<typename T>
struct get_tag
{
  tag_t tag;
};

template<>
struct get_tag<node_expr>
{
  tag_t tag = tag_t::Expr;
};

template<>
struct get_tag<token_t>
{
  tag_t tag = tag_t::Token;
};

template<>
struct get_tag<or_t>
{
  tag_t tag = tag_t::Or;
};

template<>
struct get_tag<cat_t>
{
  tag_t tag = tag_t::And;
};

template<>
struct get_tag<star_t>
{
  tag_t tag = tag_t::Star;
};

template<>
struct get_tag<nonterm_t>
{
  tag_t tag = tag_t::NonTerm;
};

}

#endif
