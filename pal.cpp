#include "print_lang.hpp"
using namespace lang;


int main(int argc, char* argv[])
{
  auto eps  = make_node<token_t>(1);
  auto zero = make_node<token_t>(2);
  auto one  = make_node<token_t>(3);

  auto lang = make_node<nonterm_t>();

  auto left  = make_nodes<cat_t>(zero, lang, zero);
  auto right = make_nodes<cat_t>(one, lang, one);

  auto top = make_nodes<or_t>(left, right, zero, one, eps);

  lang->as<nonterm_t>().left = top;
  // L = '0' L '0' | '1' L '1' | '0' | '1' | e

  std::cout << "\n";

  auto uno = std::make_pair(2, "0");
  auto dos = std::make_pair(3, "1");

  run({uno, dos, dos, dos}, lang);

  std::cout << "\n\n----------\n\n";

  run({uno, dos, uno, uno, dos, uno}, lang);

  return 0;
}
