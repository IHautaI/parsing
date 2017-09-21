#include <sstream>

#include "lang.hpp"
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
