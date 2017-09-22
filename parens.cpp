#include "print_lang.hpp"
using namespace lang;


int main(int argc, char* argv[])
{
  auto eps     = make_node<token_t>(1);
  auto left_p  = make_node<token_t>(2);
  auto right_p = make_node<token_t>(3);

  auto lang = make_node<nonterm_t>();

  auto inner = make_nodes<cat_t>(lang, left_p, lang, right_p);
  auto top = make_node<or_t>(inner, eps);

  lang->as<nonterm_t>().left = top;
  // L = L '(' L ')' | e

  std::cout << "\n";

  auto uno = std::make_pair(2, "(");
  auto dos = std::make_pair(3, ")");

  // run({uno, dos, dos, dos}, lang);

  // std::cout << "\n\n----------\n\n";

  // run({uno, uno, dos, uno, dos, dos}, lang);
  run({uno, dos}, lang);

  return 0;
}
