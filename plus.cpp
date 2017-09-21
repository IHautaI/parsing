#include "lang.hpp"
#include <sstream>

using namespace lang;

auto print(const std::string& s, std::shared_ptr<node_t>& x)
{
  for(auto& x : s)
  {
    std::cout << "D_{" << x << "} ";
  }
  std::cout << "L" << "\n";
  std::cout << "nullable?: " << x->null() << "\n\n";
}

auto run(const std::initializer_list<std::pair<int, std::string>>& lst,
         std::shared_ptr<node_t>& lang)
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
  auto one  = make_node<token_t>(2);
  auto plus = make_node<token_t>(3);

  auto lang = make_node<nonterm_t>();

  auto inner = make_nodes<cat_t>(lang, plus, lang);
  auto top   = make_node<or_t>(inner, one);

  lang->as<nonterm_t>().left = top;
  // L = L '+' L | '1'

  std::cout << "\n";

  auto uno = std::make_pair(2, "1");
  auto dos = std::make_pair(3, "+");

  run({uno, dos, dos, dos}, lang);

  std::cout << "\n\n----------\n\n";

  run({uno, dos, uno, dos, uno}, lang);


  return 0;
}
