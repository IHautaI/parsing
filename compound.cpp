#include "lang.hpp"

namespace lang {

// ------------- OPERATOR() ------------------------


  auto or_t::operator()(std::vector<std::unique_ptr<S_node>>& res) -> void
  {
    (*left)(res);
    (*right)(res);

    auto x = res.end() - 2;
    auto y = res.end() - 1;

     if( (*x)->empty )
    {
      res.erase(x);
    } else if( (*y)->empty )
    {
      res.erase(y);
    }
  }


  auto cat_t::operator()(std::vector<std::unique_ptr<S_node>>& res) -> void
  {
    (*left)(res);
    (*right)(res);

    auto x = res.end() - 2;
    auto y = res.end() - 1;

    if( !((*x)->empty || (*y)->empty) )
    {
      auto* p = new S_node(move(*x), move(*y));
      res.erase(x, res.end());
      res.emplace_back(p);

    } else if( (*y)->empty )
    {
      res.erase(y);
    } else
    {
      res.erase(x);
    }
  }


  auto star_t::operator()(std::vector<std::unique_ptr<S_node>>& res) -> void
  {
    res.emplace_back(new S_node());
  }


  // ------------ DERIVATIVES -------------------------


  auto or_t::deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>
  {
    return make_node<or_t>(make_node<node_expr>(left,  i),
                           make_node<node_expr>(right, i));
  }


  auto cat_t::deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>
  {
    auto a = make_node<node_expr>(left,  i);
    auto b = make_node<node_expr>(right, i);

    if( left->null() )
    {
      if( left->type() == -6 )         // (accept x) -> (accept x')
      {
        return make_node<cat_t>(left, b);

      } else                           // (a b) -> ( (a' b) | (b') )
      {
        return make_node<or_t>(make_node<cat_t>(a, right), b);
      }
    }

    return make_node<cat_t>(a, right); // (a b) -> (a' b)
  }


  auto star_t::deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>
  {
    return make_node<cat_t>(make_node<node_expr>(left, i),
                            std::make_shared<node_t>(this, tag_t::Star));
  }


// --------------- NULL --------------------------


  auto or_t::null() -> bool
  {
    return left->null() || right->null();
  }


  auto cat_t::null() -> bool
  {
    return left->null() && right->null();
  }


// -------------- OTHER ------------------------

auto or_t::str(std::string& s) -> void
{
  left->str(s);
  s += " | ";
  right->str(s);
}

auto cat_t::str(std::string& s) -> void
{
  left->str(s);
  right->str(s);
}

auto star_t::str(std::string& s) -> void
{
  s += "(* ";
  left->str(s);
  s += " )";
}


}
