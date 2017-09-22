/*
*  Defines a generic node_t type
*  to contain nodes via unique_ptr<void>
*  with a custom deleter deleter_t
*  and type-tags (an enum)
*/

#ifndef NODE_T_HPP
#define NODE_T_HPP

#include <functional>
#include <vector>

#include "Node.hpp"

namespace lang {

class node_expr;
class accept_t;
class token_t;
class or_t;
class cat_t;
class star_t;
class nonterm_t;

enum class tag_t
{
  Null,
  Expr,
  Accept,
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

  auto operator()(void* x) -> void;

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

  auto operator()(std::vector<std::unique_ptr<S_node>>& res) -> void;

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>;

  auto null() -> bool;

  auto type() -> int;

  auto str(std::string& s) -> void;

  template<typename T>
  auto as() -> T&
  {
    return *static_cast<T*>(node.get());
  }

  template<typename T>
  node_t(T* x, const tag_t& t)
  : node(static_cast<void*>(x), deleter_t(t))
  , tag(t)
  {}

  node_t()
  : node(nullptr, deleter_t(tag_t::Null))
  , tag(tag_t::Null)
  {}

  ~node_t(){}
};


template<typename T>
struct get_tag
{
  tag_t tag = tag_t::Null;
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

template<>
struct get_tag<accept_t>
{
  tag_t tag = tag_t::Accept;
};

}

#endif
