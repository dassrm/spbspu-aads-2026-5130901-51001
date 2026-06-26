#ifndef SMIRNOVA_HASH_TABLE_HPP
#define SMIRNOVA_HASH_TABLE_HPP

#include <utility>
#include <cstddef>
#include <stdexcept>

namespace smirnova {

  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
  public:
    struct Node {
      Key key;
      Value value;
      Node* next;
      Node(const Key& k, const Value& v) : key(k), value(v), next(nullptr) {}
    };

    HashTable(size_t slots, Hash h = Hash(), Equal eq = Equal());
    ~HashTable();

    void add(const Key& k, const Value& v);
    Value drop(const Key& k);
    bool has(const Key& k) const;
    Value get(const Key& k) const;
    void rehash(size_t slots);
    size_t load_factor() const;
    size_t max_chain_length() const;

    template< class H >
    class Iterator {
    public:
      Iterator(Node** table, size_t slot, size_t slots, Node* node);
      std::pair< const Key&, Value& > operator*();
      Iterator& operator++();
      bool operator==(const Iterator& other) const;
      bool operator!=(const Iterator& other) const;

    private:
      Node** table_;
      size_t current_slot_;
      size_t total_slots_;
      Node* current_node_;
    };

    template< class H >
    class ConstIterator {
    public:
      ConstIterator(const Node* const* table, size_t slot, size_t slots, const Node* node);
      std::pair< const Key&, const Value& > operator*() const;
      ConstIterator& operator++();
      bool operator==(const ConstIterator& other) const;
      bool operator!=(const ConstIterator& other) const;

    private:
      const Node* const* table_;
      size_t current_slot_;
      size_t total_slots_;
      const Node* current_node_;
    };

    Iterator< Key > begin();
    Iterator< Key > end();
    ConstIterator< Key > cbegin() const;
    ConstIterator< Key > cend() const;

    size_t size() const;
    bool empty() const;

  private:
    Node** table_;
    size_t slots_;
    size_t size_;
    Hash hash_;
    Equal equal_;

    void clear();
    size_t hash_function(const Key& k) const;
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots, Hash h, Equal eq)
    : slots_(slots), size_(0), hash_(h), equal_(eq) {
    table_ = new Node*[slots_];
    for (size_t i = 0; i < slots_; ++i) {
      table_[i] = nullptr;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::~HashTable() {
    clear();
    delete[] table_;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::clear() {
    for (size_t i = 0; i < slots_; ++i) {
      Node* node = table_[i];
      while (node != nullptr) {
        Node* temp = node;
        node = node->next;
        delete temp;
      }
      table_[i] = nullptr;
    }
    size_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::hash_function(const Key& k) const {
    return hash_(k) % slots_;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key& k, const Value& v) {
    size_t idx = hash_function(k);
    Node* node = table_[idx];
    while (node != nullptr) {
      if (equal_(node->key, k)) {
        node->value = v;
        return;
      }
      node = node->next;
    }
    Node* new_node = new Node(k, v);
    new_node->next = table_[idx];
    table_[idx] = new_node;
    size_++;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value HashTable< Key, Value, Hash, Equal >::drop(const Key& k) {
    size_t idx = hash_function(k);
    Node* node = table_[idx];
    Node* prev = nullptr;
    while (node != nullptr) {
      if (equal_(node->key, k)) {
        Value v = node->value;
        if (prev == nullptr) {
          table_[idx] = node->next;
        } else {
          prev->next = node->next;
        }
        delete node;
        size_--;
        return v;
      }
      prev = node;
      node = node->next;
    }
    throw std::runtime_error("Key not found");
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::has(const Key& k) const {
    size_t idx = hash_function(k);
    Node* node = table_[idx];
    while (node != nullptr) {
      if (equal_(node->key, k)) {
        return true;
      }
      node = node->next;
    }
    return false;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value HashTable< Key, Value, Hash, Equal >::get(const Key& k) const {
    size_t idx = hash_function(k);
    Node* node = table_[idx];
    while (node != nullptr) {
      if (equal_(node->key, k)) {
        return node->value;
      }
      node = node->next;
    }
    return Value();
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t slots) {
    Node** old_table = table_;
    size_t old_slots = slots_;
    slots_ = slots;
    size_ = 0;
    table_ = new Node*[slots_];
    for (size_t i = 0; i < slots_; ++i) {
      table_[i] = nullptr;
    }
    for (size_t i = 0; i < old_slots; ++i) {
      Node* node = old_table[i];
      while (node != nullptr) {
        add(node->key, node->value);
        Node* temp = node;
        node = node->next;
        delete temp;
      }
    }
    delete[] old_table;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::load_factor() const {
    return size_ / (slots_ > 0 ? slots_ : 1);
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::max_chain_length() const {
    size_t max_len = 0;
    for (size_t i = 0; i < slots_; ++i) {
      size_t len = 0;
      Node* node = table_[i];
      while (node != nullptr) {
        len++;
        node = node->next;
      }
      if (len > max_len) {
        max_len = len;
      }
    }
    return max_len;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::empty() const {
    return size_ == 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  template< class H >
  HashTable< Key, Value, Hash, Equal >::Iterator< H >::Iterator(Node** table, size_t slot, size_t slots, Node* node)
    : table_(table), current_slot_(slot), total_slots_(slots), current_node_(node) {}

  template< class Key, class Value, class Hash, class Equal >
  template< class H >
  std::pair< const Key&, Value& > HashTable< Key, Value, Hash, Equal >::Iterator< H >::operator*() {
    return std::make_pair(std::cref(current_node_->key), std::ref(current_node_->value));
  }

  template< class Key, class Value, class Hash, class Equal >
  template< class H >
  typename HashTable< Key, Value, Hash, Equal >::template Iterator< H >& HashTable< Key, Value, Hash, Equal >::Iterator< H >::operator++() {
    if (current_node_ != nullptr) {
      current_node_ = current_node_->next;
    }
    while (current_node_ == nullptr && current_slot_ < total_slots_ - 1) {
      current_slot_++;
      current_node_ = table_[current_slot_];
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  template< class H >
  bool HashTable< Key, Value, Hash, Equal >::Iterator< H >::operator==(const Iterator& other) const {
    return current_node_ == other.current_node_;
  }

  template< class Key, class Value, class Hash, class Equal >
  template< class H >
  bool HashTable< Key, Value, Hash, Equal >::Iterator< H >::operator!=(const Iterator& other) const {
    return current_node_ != other.current_node_;
  }

  template< class Key, class Value, class Hash, class Equal >
  template< class H >
  HashTable< Key, Value, Hash, Equal >::ConstIterator< H >::ConstIterator(const Node* const* table, size_t slot, size_t slots, const Node* node)
    : table_(table), current_slot_(slot), total_slots_(slots), current_node_(node) {}

  template< class Key, class Value, class Hash, class Equal >
  template< class H >
  std::pair< const Key&, const Value& > HashTable< Key, Value, Hash, Equal >::ConstIterator< H >::operator*() const {
    return std::make_pair(std::cref(current_node_->key), std::cref(current_node_->value));
  }

  template< class Key, class Value, class Hash, class Equal >
  template< class H >
  typename HashTable< Key, Value, Hash, Equal >::template ConstIterator< H >& HashTable< Key, Value, Hash, Equal >::ConstIterator< H >::operator++() {
    if (current_node_ != nullptr) {
      current_node_ = current_node_->next;
    }
    while (current_node_ == nullptr && current_slot_ < total_slots_ - 1) {
      current_slot_++;
      current_node_ = table_[current_slot_];
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  template< class H >
  bool HashTable< Key, Value, Hash, Equal >::ConstIterator< H >::operator==(const ConstIterator& other) const {
    return current_node_ == other.current_node_;
  }

  template< class Key, class Value, class Hash, class Equal >
  template< class H >
  bool HashTable< Key, Value, Hash, Equal >::ConstIterator< H >::operator!=(const ConstIterator& other) const {
    return current_node_ != other.current_node_;
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::template Iterator< Key > HashTable< Key, Value, Hash, Equal >::begin() {
    for (size_t i = 0; i < slots_; ++i) {
      if (table_[i] != nullptr) {
        return Iterator< Key >(table_, i, slots_, table_[i]);
      }
    }
    return Iterator< Key >(table_, slots_ - 1, slots_, nullptr);
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::template Iterator< Key > HashTable< Key, Value, Hash, Equal >::end() {
    return Iterator< Key >(table_, slots_ - 1, slots_, nullptr);
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::template ConstIterator< Key > HashTable< Key, Value, Hash, Equal >::cbegin() const {
    for (size_t i = 0; i < slots_; ++i) {
      if (table_[i] != nullptr) {
        return ConstIterator< Key >(table_, i, slots_, table_[i]);
      }
    }
    return ConstIterator< Key >(table_, slots_ - 1, slots_, nullptr);
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::template ConstIterator< Key > HashTable< Key, Value, Hash, Equal >::cend() const {
    return ConstIterator< Key >(table_, slots_ - 1, slots_, nullptr);
  }

}

#endif
