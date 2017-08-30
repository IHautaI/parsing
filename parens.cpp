#include "lang.hpp"
#include <sstream>

using namespace lang;

auto print(const std::string& s, std::shared_ptr<Node>& x)
{
  for(auto& x : s)
  {
    std::cout << "D_{" << x << "} ";
  }
  std::cout << "L" << "\n";
  std::cout << "nullable?: " << x->null() << "\n\n";
}


auto run(const std::initializer_list<std::pair<int, std::string>>& lst,
         std::shared_ptr<Node>& lang)
         -> void
{
  std::stringstream ss;
  auto x = lang;
  for(auto& c : lst)
  {
    x = x->deriv(c);
    ss << c.first;
    print(ss.str(), x);
  }
}

int main(int argc, char* argv[])
{
  auto eps     = make_node<token_t>(1);
  auto left_p  = make_node<token_t>(2);
  auto right_p = make_node<token_t>(3);

  auto lang = make_node<nonterm_t>();

  auto inner = make_nodes<cat_t>(lang, left_p, lang, right_p);
  auto top = make_node<or_t>(inner, eps);

  dynamic_cast<nonterm_t*>(lang.get())->left = top;
  // L = L '(' L ')' | e

  std::cout << "\n";

  auto uno = std::make_pair(2, "(");
  auto dos = std::make_pair(3, ")");

  run({uno, dos, dos, dos}, lang);

  std::cout << "\n\n----------\n\n";

  run({uno, uno, dos, uno, dos, dos}, lang);

  return 0;
}
