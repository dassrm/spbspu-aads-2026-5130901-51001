#ifndef SMIRNOVA_HASH_TABLE_HPP
#define SMIRNOVA_HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <list>
#include <memory>
#include <utility>
#include <vector>

namespace smirnova {

  template< typename Key, typename Value, typename Hash = std::hash< Key > >
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

      KeyValue& operator*() const
      {
        return *inner_;
      }

      KeyValue* operator->() const
      {
        return std::addressof(*inner_);
      }

      iterator& operator++()
      {
        ++inner_;
        skipEmptyBuckets();
        return *this;
      }

      bool operator==(const iterator& other) const
      {
        if (atEnd() && other.atEnd()) {
          return true;
        }
        return buckets_ == other.buckets_ && index_ == other.index_ && inner_ == other.inner_;
      }

      bool operator!=(const iterator& other) const
      {
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

      bool atEnd() const
      {
        return buckets_ == nullptr || index_ >= buckets_->size();
      }

      void skipEmptyBuckets()
      {
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

    iterator begin()
    {
      return iterator(std::addressof(buckets_), 0);
    }

    iterator end()
    {
      return iterator(std::addressof(buckets_), buckets_.size());
    }

    iterator find(const Key& key)
    {
      std::size_t index = bucketIndex(key);
      Bucket& bucket = buckets_[index];
      for (typename Bucket::iterator it = bucket.begin(); it != bucket.end(); ++it) {
        if (it->first == key) {
          return iterator(std::addressof(buckets_), index, it);
        }
      }
      return end();
    }

    bool insert(const Key& key, const Value& value)
    {
      if (find(key) != end()) {
        return false;
      }
      if (loadFactorExceeded(size_ + 1)) {
        rehash();
      }
      std::size_t index = bucketIndex(key);
      buckets_[index].push_back(KeyValue(key, value));
      ++size_;
      return true;
    }

    bool erase(const Key& key)
    {
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

    std::size_t size() const
    {
      return size_;
    }

    bool empty() const
    {
      return size_ == 0;
    }

  private:
    static const std::size_t INITIAL_BUCKET_COUNT = 8;
    static const std::size_t MAX_LOAD_PERCENT = 75;

    Buckets buckets_;
    std::size_t size_;
    Hash hasher_;

    std::size_t bucketIndex(const Key& key) const
    {
      return hasher_(key) % buckets_.size();
    }

    bool loadFactorExceeded(std::size_t futureSize) const
    {
      return futureSize * 100 > buckets_.size() * MAX_LOAD_PERCENT;
    }

    void rehash()
    {
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
