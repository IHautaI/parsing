#include <sstream>
#include <algorithm>

#include "lang.hpp"
using namespace lang;

auto print(const std::string& s, std::shared_ptr<node_t>& x) -> void
{
  for( auto z = s.rbegin(); z < s.rend(); z++)
  {
    std::cout << "D_" << *z << " ";
  }
  std::cout << "L" << "\n";
  std::cout << "nullable?: " << x->null() << "\n\n";
}


auto print_node(std::unique_ptr<S_node>& x) -> bool
{
  auto ret = true;
  std::cout << " ( ";

  if( !x->empty )
  {
    print_node(x->left);
    std::cout << x->value.first;
    print_node(x->right);
  } else
  {
    ret = false;
  }
  std::cout << " ) ";

  return ret;
}


auto print(std::vector<std::unique_ptr<S_node>>& res) -> void
{

  res.erase(std::remove_if(res.begin(),
                           res.end(),
                           [](auto& x)
                           {
                             return x->empty;
                           }),
            res.end());

  std::cout << "Parse Trees: " << res.size() << "\n\n";
  for( auto& x : res )
  {
    print_node(x);
    std::cout << "\n\n";
  }
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
    ss << c.second;
    print(ss.str(), x);
    auto s = std::string();
    x->str(s);
    std::cout << s << "\n\n";
  }

  if( x->null() )
  {
    auto res = std::vector<std::unique_ptr<S_node>>();
    (*x)(res);

    print(res);
  }
}
