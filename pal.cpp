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
  auto eps  = make_node<token_t>(1);
  auto zero = make_node<token_t>(2);
  auto one  = make_node<token_t>(3);

  auto lang = make_node<nonterm_t>();

  auto left  = make_nodes<cat_t>(zero, lang, zero);
  auto right = make_nodes<cat_t>(one, lang, one);

  auto top = make_nodes<or_t>(left, right, zero, one, eps);

  dynamic_cast<nonterm_t*>(lang.get())->left = top;
  // L = '0' L '0' | '1' L '1' | '0' | '1' | e

  std::cout << "\n";

  auto uno = std::make_pair(2, "0");
  auto dos = std::make_pair(3, "1");

  run({uno, dos, dos, dos}, lang);

  std::cout << "\n\n----------\n\n";

  run({uno, dos, uno, uno, dos, uno}, lang);

  return 0;
}
