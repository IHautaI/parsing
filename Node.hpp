#ifndef __NODE_HPP
#define __NODE_HPP

#include <memory>
#include <utility>
#include <vector>

namespace lang {

  struct S_node {
    std::pair<int, std::string> value;
    std::unique_ptr<S_node> left;
    std::unique_ptr<S_node> right;

    bool empty;

    S_node(const std::pair<int, std::string>& value)
    : value(value)
    , left(new S_node())
    , right(new S_node())
    , empty(false)
    {}

    S_node(std::unique_ptr<S_node> left, std::unique_ptr<S_node> right)
    : left(move(left))
    , right(move(right))
    , empty(left->empty && right->empty )
    {}

    S_node()
    : empty(true)
    {}

    ~S_node(){}
  };

}

#endif
