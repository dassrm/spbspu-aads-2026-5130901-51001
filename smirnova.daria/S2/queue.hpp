#ifndef SMIRNOVA_QUEUE_HPP
#define SMIRNOVA_QUEUE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include "list.hpp"

namespace smirnova {

template< class T >
class Queue {
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
      throw std::runtime_error("drop() on empty queue");
    }
    T val = std::move(list_.front());
    list_.popFront();
    return val;
  }

  const T& front() const
  {
    if (empty()) {
      throw std::runtime_error("front() on empty queue");
    }
    return list_.front();
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
