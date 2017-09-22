#include "lang.hpp"

namespace lang {

  auto deleter_t::operator()(void* x) -> void
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
      case tag_t::Accept:
        delete static_cast<accept_t*>(x);
        break;
      case tag_t::Null:
        break;
    }
  }

  auto node_t::operator()(std::vector<std::unique_ptr<S_node>>& res) -> void
  {
    switch(tag)
    {
      case tag_t::Expr:
        (*static_cast<node_expr*>(node.get()))(res);
        break;
      case tag_t::Token:
        (*static_cast<token_t*>(node.get()))(res);
        break;
      case tag_t::And:
        (*static_cast<cat_t*>(node.get()))(res);
        break;
      case tag_t::Or:
        (*static_cast<or_t*>(node.get()))(res);
        break;
      case tag_t::Star:
        (*static_cast<star_t*>(node.get()))(res);
        break;
      case tag_t::NonTerm:
        (*static_cast<nonterm_t*>(node.get()))(res);
        break;
      case tag_t::Accept:
        (*static_cast<accept_t*>(node.get()))(res);
        break;
      case tag_t::Null:
        res.emplace_back(new S_node());
        break;
    }
  }

  auto node_t::deriv(const std::pair<int, std::string>& i) -> std::shared_ptr<node_t>
  {
    switch(tag)
    {
      case tag_t::Expr:
        return static_cast<node_expr*>(node.get())->deriv(i);
        break;
      case tag_t::Token:
        return static_cast<token_t*>(node.get())->deriv(i);
        break;
      case tag_t::And:
        return static_cast<cat_t*>(node.get())->deriv(i);
        break;
      case tag_t::Or:
        return static_cast<or_t*>(node.get())->deriv(i);
        break;
      case tag_t::Star:
        return static_cast<star_t*>(node.get())->deriv(i);
        break;
      case tag_t::NonTerm:
        return static_cast<nonterm_t*>(node.get())->deriv(i);
        break;
      case tag_t::Accept:
        return empty();
        break;
      case tag_t::Null:
        return std::shared_ptr<node_t>(this);
        break;
    }
  }

  auto node_t::null() -> bool
  {
    switch(tag)
    {
      case tag_t::Expr:
        return static_cast<node_expr*>(node.get())->null();
        break;
      case tag_t::Token:
        return static_cast<token_t*>(node.get())->null();
        break;
      case tag_t::And:
        return static_cast<cat_t*>(node.get())->null();
        break;
      case tag_t::Or:
        return static_cast<or_t*>(node.get())->null();
        break;
      case tag_t::Star:
        return true;
        break;
      case tag_t::NonTerm:
        return static_cast<nonterm_t*>(node.get())->null();
        break;
      case tag_t::Accept:
        return true;
        break;
      case tag_t::Null:
        return false;
        break;
    }
  }

  auto node_t::type() -> int
  {
    switch(tag)
    {
      case tag_t::Expr:
        return static_cast<node_expr*>(node.get())->type();
        break;
      case tag_t::Token:
        return static_cast<token_t*>(node.get())->type();
        break;
      case tag_t::And:
        return static_cast<cat_t*>(node.get())->type();
        break;
      case tag_t::Or:
        return static_cast<or_t*>(node.get())->type();
        break;
      case tag_t::Star:
        return static_cast<star_t*>(node.get())->type();
        break;
      case tag_t::NonTerm:
        return static_cast<nonterm_t*>(node.get())->type();
        break;
      case tag_t::Accept:
        return static_cast<accept_t*>(node.get())->type();
        break;
      case tag_t::Null:
        return 0;
        break;
    }
  }

  auto node_t::str(std::string& s) -> void
  {
    switch(tag)
    {
      case tag_t::Expr:
        return static_cast<node_expr*>(node.get())->str(s);
        break;
      case tag_t::Token:
        return static_cast<token_t*>(node.get())->str(s);
        break;
      case tag_t::And:
        return static_cast<cat_t*>(node.get())->str(s);
        break;
      case tag_t::Or:
        return static_cast<or_t*>(node.get())->str(s);
        break;
      case tag_t::Star:
        return static_cast<star_t*>(node.get())->str(s);
        break;
      case tag_t::NonTerm:
        return static_cast<nonterm_t*>(node.get())->str(s);
        break;
      case tag_t::Accept:
        return static_cast<accept_t*>(node.get())->str(s);
        break;
      case tag_t::Null:
        return;
        break;
    }
  }

  auto empty() -> std::shared_ptr<node_t>
  {
    return std::make_shared<node_t>();
  }

}
