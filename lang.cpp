#include "lang.hpp"

using namespace lang;

auto node_expr::deriv(int i) -> std::shared_ptr<Node>
{
  if( !resolved )
  {
    eval();
  }
  return obj->deriv(i);
}


auto node_expr::eval() -> void
{
  obj = left->deriv(wrt);
}



auto token_t::deriv(int i) -> std::shared_ptr<Node>
{
  if( type == 0 || type == 1 || type != i )
  {
    return make_node<token_t>(0);
  }

  return make_node<token_t>(1);
}



// needs to contain itself potentially
auto nonterm_t::deriv(int i) -> std::shared_ptr<Node>
{
  if( last.first != i ){
    last = std::make_pair(i, make_node<nonterm_t>(make_node<node_expr>(left,
                                                                       i)));
  }

  return last.second;
}


auto cat_t::deriv(int i) -> std::shared_ptr<Node>
{
  if( left->null() )
  {
    return make_node<or_t>(
              make_node<cat_t>(make_node<node_expr>(left, i), right),
              make_node<node_expr>(right, i));
  }

  return make_node<cat_t>(make_node<node_expr>(left, i), right);
}

auto or_t::deriv(int i) -> std::shared_ptr<Node>
{
  return make_node<or_t>(make_node<node_expr>(left, i),
                         make_node<node_expr>(right, i));
}


auto star_t::deriv(int i) -> std::shared_ptr<Node>
{
  return make_node<cat_t>(make_node<node_expr>(left, i),
                          std::shared_ptr<Node>(
                            dynamic_cast<Node*>(this)));
}
