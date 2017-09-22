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


template<typename T, typename... Args>
auto make_node(Args&&... args) -> std::shared_ptr<node_t>;


class accept_t : public Node
{
  std::pair<int, std::string> value;

public:

  auto str(std::string& s) -> void;

  auto operator()(std::vector<std::unique_ptr<S_node>>& res) -> void;

  accept_t(const std::pair<int, std::string>& value)
  : Node(-6, true)
  , value(value)
  {}

  ~accept_t(){}
};


class token_t : public Node
{
public:

  auto operator()(std::vector<std::unique_ptr<S_node>>& res) -> void;

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>;

  auto null() -> bool;

  auto str(std::string& s) -> void;

  token_t(const int i)
  : Node(i, i == 1)
  {}

  ~token_t(){}
};


/*
*  Delayed-evaluation derivative
*/
class node_expr : public Node
{
  std::shared_ptr<node_t> left;
  std::shared_ptr<node_t> obj;
  std::pair<int, std::string> wrt;
  bool resolved;
  bool culled;

  auto eval() -> void;

public:

  auto operator()(std::vector<std::unique_ptr<S_node>>& res) -> void;

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>;

  auto null() -> bool;

  auto str(std::string& s) -> void;

  node_expr(const std::shared_ptr<node_t>& left, const std::pair<int, std::string>& i)
  : Node(-5)
  , left(left)
  , wrt(i)
  , resolved(false)
  , culled(false)
  {}

  ~node_expr(){}
};


class nonterm_t : public Node
{
  bool done;
  bool culled;

  std::pair<std::pair<int, std::string>, std::shared_ptr<node_t>> last;

public:

  std::shared_ptr<node_t> left;

  auto operator()(std::vector<std::unique_ptr<S_node>>& res) -> void;

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>;

  auto null() -> bool;

  auto str(std::string& s) -> void;

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


class or_t : public Node
{
  std::shared_ptr<node_t> left;
  std::shared_ptr<node_t> right;

public:

  auto operator()(std::vector<std::unique_ptr<S_node>>& res) -> void;

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>;

  auto null() -> bool;

  auto str(std::string& s) -> void;

  or_t(const std::shared_ptr<node_t>& l,
       const std::shared_ptr<node_t>& r)
  : Node(-2)
  , left(l)
  , right(r)
  {}

  ~or_t(){};
};


class cat_t : public Node
{
  std::shared_ptr<node_t> left;
  std::shared_ptr<node_t> right;

public:

  auto operator()(std::vector<std::unique_ptr<S_node>>& res) -> void;

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>;

  auto null() -> bool;

  auto str(std::string& s) -> void;

  cat_t(const std::shared_ptr<node_t>& l,
        const std::shared_ptr<node_t>& r)
  : Node(-3)
  , left(l)
  , right(r)
  {}

  ~cat_t(){}
};


class star_t : public Node
{
  std::shared_ptr<node_t> left;

public:

  auto operator()(std::vector<std::unique_ptr<S_node>>& res) -> void;

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>;

  auto null() -> bool;

  auto str(std::string& s) -> void;

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

auto empty() -> std::shared_ptr<node_t>;

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
