#include "print_lang.hpp"
using namespace lang;


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
