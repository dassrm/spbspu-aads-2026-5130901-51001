#ifndef SMIRNOVA_STACK_HPP
#define SMIRNOVA_STACK_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include "list.hpp"

namespace smirnova {

template< class T >
class Stack {
public:
  void push(const T& val)
  {
    list_.pushBack(val);
  }

  void push(T&& val)
  {
    list_.pushBack(std::move(val));
  }

  T drop()
  {
    if (empty()) {
      throw std::runtime_error("drop() on empty stack");
    }
    T val = std::move(list_.back());
    list_.popBack();
    return val;
  }

  const T& top() const
  {
    if (empty()) {
      throw std::runtime_error("top() on empty stack");
    }
    return list_.back();
  }

  bool empty() const
  {
    return list_.empty();
  }

  std::size_t size() const
  {
    return list_.size();
  }

private:
  List< T > list_;
};

}

#endif
