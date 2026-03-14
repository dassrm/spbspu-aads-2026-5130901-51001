// list.hpp
#ifndef SMIRNOVA_LIST_HPP
#define SMIRNOVA_LIST_HPP

#include <cstddef>
#include <utility>

namespace smirnova {

namespace detail {

struct node_base_t {
  node_base_t* next_;
  node_base_t* prev_;

  node_base_t():
    next_(this),
    prev_(this)
  {}
};

template< class T >
struct node_t: node_base_t {
  T value_;

  template< class U >
  explicit node_t(U&& val):
    node_base_t(),
    value_(std::forward< U >(val))
  {}
};

}

template< class T >
class List {
public:
  List():
    sentinel_(new detail::node_base_t()),
    size_(0)
  {}

  ~List()
  {
    clear();
    delete sentinel_;
  }

  bool empty() const
  {
    return size_ == 0;
  }

  std::size_t size() const
  {
    return size_;
  }

private:
  detail::node_base_t* sentinel_;
  std::size_t size_;
};

}

#endif
