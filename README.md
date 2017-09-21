# parsing
parsing library fun


lang.hpp/cpp -> library

pal.cpp, parens.cpp, plus.cpp -> tests
make pal/parens/plus to make the tests
run them with ./(pal/parens/plus)

pal -> palindromes on {0, 1}
parens -> matched parens
plus ->  L = L + L | 1


# todo
* Parser class - holds shared_ptr to created language,
    takes incoming tokens and applies derivative,
    creates parse tree(s)
* small DSL for simplified language creation
