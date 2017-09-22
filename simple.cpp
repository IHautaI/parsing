#include "lang.hpp"

namespace lang {

// ------------ OPERATOR() ----------------------


auto accept_t::operator()(std::vector<std::unique_ptr<S_node>>& res) -> void
{
  res.emplace_back(new S_node(value));
}


auto token_t::operator()(std::vector<std::unique_ptr<S_node>>& res) -> void
{
  res.emplace_back(new S_node());
}


auto node_expr::operator()(std::vector<std::unique_ptr<S_node>>& res) -> void
{
  if( !resolved )
  {
    eval();
  }

  if( !culled )
  {
    (*obj)(res);
  } else
  {
    res.emplace_back(new S_node());
  }
}


auto nonterm_t::operator()(std::vector<std::unique_ptr<S_node>>& res) -> void
{
  if( !culled )
  {
    culled = true;
    (*left)(res);
    culled = false;
  } else
  {
    res.emplace_back(new S_node());
  }
}


// --------------- DERIVATIVES -----------------------


auto token_t::deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>
{
  if( type() == 1 || type() != i.first )
  {
    return empty();
  }

  return make_node<accept_t>(i);
}


auto node_expr::deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>
{
  if( !resolved )
  {
    eval();
  }

  return obj->deriv(i);
}


auto nonterm_t::deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>
{
  if( last.first.first != i.first )
  {
    last = std::make_pair(i, make_node<nonterm_t>(
               make_node<node_expr>(left, i)));
  }

  return last.second;
}


// --------  NULL ---------------------


auto token_t::null() -> bool
{
  return type() == 1;
}


auto node_expr::null() -> bool
{
  if( !resolved )
  {
    eval();
  }

  return obj->null();
}


auto nonterm_t::null() -> bool
{
  if( !done )
  { // uses false as default
    // when finding itself in it's contained nodes
    done = true;
    nullable = left->null();
  }

  return nullable;
}


// --------- OTHER -----------------


auto node_expr::eval() -> void
{
  obj = left->deriv(wrt);
  resolved = true;
}


auto accept_t::str(std::string& s) -> void
{
  s += "(eps ";
  s += value.first;
  s += " )";
}


auto token_t::str(std::string& s) -> void
{
  s += "(tok ";
  s +=  type();
  s += " )";
}

auto node_expr::str(std::string& s) -> void
{
  if( !resolved )
  {
    eval();
  }

  obj->str(s);
}

auto nonterm_t::str(std::string& s) -> void
{
  if( !culled )
  {
    culled = true;
    left->str(s);
    culled = false;
  }
}

}
