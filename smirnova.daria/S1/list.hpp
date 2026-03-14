#ifndef SMIRNOVA_LIST_HPP
#define SMIRNOVA_LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <type_traits>
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

template< class T, bool IsConst >
class ListIter {
public:
  using ref = typename std::conditional< IsConst, const T&, T& >::type;
  using ptr = typename std::conditional< IsConst, const T*, T* >::type;

  ListIter():
    node_(nullptr)
  {}

  template< bool C = IsConst, class = typename std::enable_if< C >::type >
  ListIter(const ListIter< T, false >& other):
    node_(other.node_)
  {}

  ref operator*() const
  {
    using node_ptr = typename std::conditional<
      IsConst,
      const detail::node_t< T >*,
      detail::node_t< T >*
    >::type;
    return static_cast< node_ptr >(node_)->value_;
  }

  ptr operator->() const
  {
    return &(**this);
  }

  ListIter& operator++()
  {
    node_ = node_->next_;
    return *this;
  }

  ListIter operator++(int)
  {
    ListIter tmp(*this);
    ++(*this);
    return tmp;
  }

  ListIter& operator--()
  {
    node_ = node_->prev_;
    return *this;
  }

  ListIter operator--(int)
  {
    ListIter tmp(*this);
    --(*this);
    return tmp;
  }

  bool operator==(const ListIter& other) const
  {
    return node_ == other.node_;
  }

  bool operator!=(const ListIter& other) const
  {
    return !(*this == other);
  }

private:
  friend class List< T >;
  friend class ListIter< T, true >;

  using base_ptr = typename std::conditional<
    IsConst,
    const detail::node_base_t*,
    detail::node_base_t*
  >::type;

  base_ptr node_;

  explicit ListIter(base_ptr node):
    node_(node)
  {}
};

template< class T >
using LIter = ListIter< T, false >;

template< class T >
using LCIter = ListIter< T, true >;

template< class T >
class List {
public:
  using iterator = LIter< T >;
  using const_iterator = LCIter< T >;

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
