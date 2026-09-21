#ifndef SMIRNOVA_HASH_TABLE_HPP
#define SMIRNOVA_HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <list>
#include <utility>
#include <vector>

namespace smirnova {

  // A generic separate-chaining hash table: Key -> Value, with a plain
  // std::list per bucket and nothing else from the standard library. It
  // grows (doubles the bucket count) whenever the load factor would rise
  // above 0.75, so it never degrades into a handful of huge buckets.
  //
  // All access goes through iterator (find/begin/end), the same way
  // std::unordered_map works: callers never see a raw pointer into the
  // table's storage.
  template < typename Key, typename Value, typename Hash = std::hash< Key > >
  class HashTable {
  public:
    using KeyValue = std::pair< const Key, Value >;

  private:
    using Bucket = std::list< KeyValue >;
    using Buckets = std::vector< Bucket >;

  public:
    class iterator {
    public:
      iterator():
        buckets_(nullptr),
        index_(0)
      {}

      KeyValue& operator*() const {
        return *inner_;
      }

      KeyValue* operator->() const {
        return &(*inner_);
      }

      iterator& operator++() {
        ++inner_;
        skipEmptyBuckets();
        return *this;
      }

      bool operator==(const iterator& other) const {
        if (atEnd() && other.atEnd()) {
          return true;
        }
        return buckets_ == other.buckets_ && index_ == other.index_ && inner_ == other.inner_;
      }

      bool operator!=(const iterator& other) const {
        return !(*this == other);
      }

    private:
      Buckets* buckets_;
      std::size_t index_;
      typename Bucket::iterator inner_;

      iterator(Buckets* buckets, std::size_t index):
        buckets_(buckets),
        index_(index)
      {
        if (!atEnd()) {
          inner_ = (*buckets_)[index_].begin();
          skipEmptyBuckets();
        }
      }

      iterator(Buckets* buckets, std::size_t index, typename Bucket::iterator inner):
        buckets_(buckets),
        index_(index),
        inner_(inner)
      {}

      bool atEnd() const {
        return buckets_ == nullptr || index_ >= buckets_->size();
      }

      void skipEmptyBuckets() {
        while (!atEnd() && inner_ == (*buckets_)[index_].end()) {
          ++index_;
          if (!atEnd()) {
            inner_ = (*buckets_)[index_].begin();
          }
        }
      }

      friend class HashTable;
    };

    HashTable():
      buckets_(INITIAL_BUCKET_COUNT),
      size_(0)
    {}

    iterator find(const Key& key) {
      std::size_t index = bucketIndex(key);
      Bucket& bucket = buckets_[index];
      for (typename Bucket::iterator it = bucket.begin(); it != bucket.end(); ++it) {
        if (it->first == key) {
          return iterator(&buckets_, index, it);
        }
      }
      return end();
    }

    iterator begin() {
      return iterator(&buckets_, 0);
    }

    iterator end() {
      return iterator(&buckets_, buckets_.size());
    }

    bool insert(const Key& key, const Value& value) {
      if (find(key) != end()) {
        return false;
      }
      double loadFactorAfter = static_cast< double >(size_ + 1) / static_cast< double >(buckets_.size());
      if (loadFactorAfter > MAX_LOAD_FACTOR) {
        rehash();
      }
      std::size_t index = bucketIndex(key);
      buckets_[index].push_back(KeyValue(key, value));
      ++size_;
      return true;
    }

    bool erase(const Key& key) {
      Bucket& bucket = buckets_[bucketIndex(key)];
      for (typename Bucket::iterator it = bucket.begin(); it != bucket.end(); ++it) {
        if (it->first == key) {
          bucket.erase(it);
          --size_;
          return true;
        }
      }
      return false;
    }

    std::size_t size() const {
      return size_;
    }

    bool empty() const {
      return size_ == 0;
    }

  private:
    static const std::size_t INITIAL_BUCKET_COUNT = 8;
    static constexpr double MAX_LOAD_FACTOR = 0.75;

    Buckets buckets_;
    std::size_t size_;
    Hash hasher_;

    std::size_t bucketIndex(const Key& key) const {
      return hasher_(key) % buckets_.size();
    }

    void rehash() {
      Buckets bigger(buckets_.size() * 2);
      for (Bucket& bucket : buckets_) {
        for (KeyValue& entry : bucket) {
          std::size_t index = hasher_(entry.first) % bigger.size();
          bigger[index].push_back(std::move(entry));
        }
      }
      buckets_ = std::move(bigger);
    }
  };

}

#endif
