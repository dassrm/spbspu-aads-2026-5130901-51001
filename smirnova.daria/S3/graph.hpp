#ifndef SMIRNOVA_GRAPH_HPP
#define SMIRNOVA_GRAPH_HPP

#include "hash_table.hpp"
#include "blake2.hpp"
#include <utility>
#include <cstring>

namespace smirnova {

  struct StringEqual {
    bool operator()(const char* a, const char* b) const {
      return std::strcmp(a, b) == 0;
    }
  };

  struct StringHash {
    size_t operator()(const char* str) const {
      Blake2 hash;
      return hash(str);
    }
  };

  struct PairHash {
    size_t operator()(const std::pair< char*, char* >& p) const {
      Blake2 hash;
      size_t h1 = hash(p.first);
      size_t h2 = hash(p.second);
      return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
  };

  struct PairEqual {
    bool operator()(const std::pair< char*, char* >& a, const std::pair< char*, char* >& b) const {
      return std::strcmp(a.first, b.first) == 0 && std::strcmp(a.second, b.second) == 0;
    }
  };

  struct EdgeList {
    int* weights;
    size_t count;
    size_t capacity;

    EdgeList() : weights(nullptr), count(0), capacity(0) {}
    ~EdgeList() { delete[] weights; }

    void add(int weight) {
      if (count >= capacity) {
        size_t new_cap = (capacity == 0) ? 10 : capacity * 2;
        int* new_weights = new int[new_cap];
        for (size_t i = 0; i < count; ++i) {
          new_weights[i] = weights[i];
        }
        delete[] weights;
        weights = new_weights;
        capacity = new_cap;
      }
      weights[count++] = weight;
    }

    bool remove(int weight) {
      for (size_t i = 0; i < count; ++i) {
        if (weights[i] == weight) {
          for (size_t j = i; j < count - 1; ++j) {
            weights[j] = weights[j + 1];
          }
          count--;
          return true;
        }
      }
      return false;
    }
  };

  class Graph {
  public:
    Graph(const char* name);
    ~Graph();

    const char* name() const;
    void add_vertex(const char* v);
    void add_edge(const char* from, const char* to, int weight);
    bool has_vertex(const char* v) const;
    bool has_edge(const char* from, const char* to, int weight) const;
    bool remove_edge(const char* from, const char* to, int weight);
    void get_outbound(const char* v, char**& vertices, int**& weights, size_t& count) const;
    void get_inbound(const char* v, char**& vertices, int**& weights, size_t& count) const;

  private:
    char* name_;
    HashTable< char*, bool, StringHash, StringEqual >* vertices_;
    HashTable< std::pair< char*, char* >, EdgeList*, PairHash, PairEqual >* edges_;

    char* copy_string(const char* str);
  };

}

#endif
