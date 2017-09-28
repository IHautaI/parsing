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


class accept_t
{
  std::pair<int, std::string> value;

public:

  auto str(std::string& s) -> void;

  auto operator()(std::vector<std::unique_ptr<S_node>>& res) -> void;

  accept_t(const std::pair<int, std::string>& value)
  : value(value)
  {}

  ~accept_t(){}
};


class token_t
{
public:
  int _type;
  auto operator()(std::vector<std::unique_ptr<S_node>>& res) -> void;

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>;

  auto null() -> bool;

  auto type() -> int
  {
    return _type;
  }

  auto str(std::string& s) -> void;

  token_t(const int i)
  : _type(i)
  {}

  ~token_t(){}
};


/*
*  Delayed-evaluation derivative
*/
class node_expr
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
  : left(left)
  , wrt(i)
  , resolved(false)
  , culled(false)
  {}

  ~node_expr(){}
};


class nonterm_t
{
  bool done;
  bool culled;
  bool nullable;

  std::pair<std::pair<int, std::string>, std::shared_ptr<node_t>> last;
  std::string name;

public:

  std::shared_ptr<node_t> left;

  auto operator()(std::vector<std::unique_ptr<S_node>>& res) -> void;

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>;

  auto null() -> bool;

  auto str(std::string& s) -> void;

  nonterm_t(const std::string& name = "L")
  : done(false)
  , culled(false)
  , nullable(false)
  , name(name)
  {}

  nonterm_t(const std::shared_ptr<node_t>& l, const std::string& name = "L")
  : left(l)
  , done(false)
  , culled(false)
  , nullable(false)
  , name(name)
  {}

  ~nonterm_t(){}
};


class or_t
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
  : left(l)
  , right(r)
  {}

  ~or_t(){};
};


class cat_t
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
  : left(l)
  , right(r)
  {}

  ~cat_t(){}
};


class star_t
{
  std::shared_ptr<node_t> left;

public:

  auto operator()(std::vector<std::unique_ptr<S_node>>& res) -> void;

  auto deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>;

  auto null() -> bool;

  auto str(std::string& s) -> void;

  star_t(const std::shared_ptr<node_t>& n)
  : left(n)
  {}

  ~star_t(){}
};


auto empty() -> std::shared_ptr<node_t>;

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
