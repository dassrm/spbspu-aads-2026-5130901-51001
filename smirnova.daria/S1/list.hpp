#ifndef SMIRNOVA_LIST_HPP
#define SMIRNOVA_LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <string>
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

  iterator begin()
  {
    return iterator(sentinel_->next_);
  }

  iterator end()
  {
    return iterator(sentinel_);
  }

  const_iterator begin() const
  {
    return const_iterator(sentinel_->next_);
  }

  const_iterator end() const
  {
    return const_iterator(sentinel_);
  }

  const_iterator cbegin() const
  {
    return begin();
  }

  const_iterator cend() const
  {
    return end();
  }

  T& front()
  {
    checkNotEmpty("front");
    return *begin();
  }

  const T& front() const
  {
    checkNotEmpty("front");
    return *begin();
  }

  T& back()
  {
    checkNotEmpty("back");
    return *iterator(sentinel_->prev_);
  }

  const T& back() const
  {
    checkNotEmpty("back");
    return *const_iterator(sentinel_->prev_);
  }

  bool empty() const
  {
    return size_ == 0;
  }

  std::size_t size() const
  {
    return size_;
  }

  void pushFront(T val)
  {
    insert(begin(), std::move(val));
  }

  void pushBack(T val)
  {
    insert(end(), std::move(val));
  }

  void popFront()
  {
    checkNotEmpty("popFront");
    erase(begin());
  }

  void popBack()
  {
    checkNotEmpty("popBack");
    erase(iterator(sentinel_->prev_));
  }

  iterator insert(iterator pos, T val)
  {
    detail::node_t< T >* newNode = new detail::node_t< T >(std::move(val));
    detail::node_base_t* curr = pos.node_;
    detail::node_base_t* prev = curr->prev_;
    newNode->next_ = curr;
    newNode->prev_ = prev;
    prev->next_ = newNode;
    curr->prev_ = newNode;
    ++size_;
    return iterator(newNode);
  }

  iterator erase(iterator pos)
  {
    if (pos == end()) {
      throw std::runtime_error("erase() on end iterator");
    }
    detail::node_base_t* curr = pos.node_;
    curr->prev_->next_ = curr->next_;
    curr->next_->prev_ = curr->prev_;
    detail::node_base_t* next = curr->next_;
    delete static_cast< detail::node_t< T >* >(curr);
    --size_;
    return iterator(next);
  }

  void clear()
  {
    while (!empty()) {
      popFront();
    }
  }

private:
  detail::node_base_t* sentinel_;
  std::size_t size_;

  void checkNotEmpty(const char* method) const
  {
    if (empty()) {
      throw std::runtime_error(std::string(method) + "() on empty list");
    }
  }
};

}

#endif
