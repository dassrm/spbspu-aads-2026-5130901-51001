#include "graph.hpp"
#include <cstring>

namespace smirnova {

  Graph::Graph(const char* name) {
    name_ = copy_string(name);
    vertices_ = new HashTable< char*, bool, StringHash, StringEqual >(16);
    edges_ = new HashTable< std::pair< char*, char* >, EdgeList*, PairHash, PairEqual >(16);
  }

  Graph::~Graph() {
    delete[] name_;
    delete vertices_;
    delete edges_;
  }

  char* Graph::copy_string(const char* str) {
    char* copy = new char[std::strlen(str) + 1];
    std::strcpy(copy, str);
    return copy;
  }

  const char* Graph::name() const {
    return name_;
  }

  void Graph::add_vertex(const char* v) {
    if (!has_vertex(v)) {
      char* v_copy = copy_string(v);
      vertices_->add(v_copy, true);
    }
  }

  void Graph::add_edge(const char* from, const char* to, int weight) {
    add_vertex(from);
    add_vertex(to);

    char* from_copy = copy_string(from);
    char* to_copy = copy_string(to);
    std::pair< char*, char* > key = std::make_pair(from_copy, to_copy);

    if (edges_->has(key)) {
      EdgeList* el = edges_->get(key);
      el->add(weight);
      delete[] from_copy;
      delete[] to_copy;
    } else {
      EdgeList* el = new EdgeList();
      el->add(weight);
      edges_->add(key, el);
    }
  }

  bool Graph::has_vertex(const char* v) const {
    return vertices_->has(v);
  }

  bool Graph::has_edge(const char* from, const char* to, int weight) const {
    std::pair< char*, char* > key = std::make_pair(const_cast< char* >(from), const_cast< char* >(to));
    if (edges_->has(key)) {
      EdgeList* el = edges_->get(key);
      for (size_t i = 0; i < el->count; ++i) {
        if (el->weights[i] == weight) {
          return true;
        }
      }
    }
    return false;
  }

  bool Graph::remove_edge(const char* from, const char* to, int weight) {
    std::pair< char*, char* > key = std::make_pair(const_cast< char* >(from), const_cast< char* >(to));
    if (edges_->has(key)) {
      EdgeList* el = edges_->get(key);
      return el->remove(weight);
    }
    return false;
  }

  void Graph::get_outbound(const char* v, char**& vertices, int**& weights, size_t& count) const {
    count = 0;
    vertices = nullptr;
    weights = nullptr;

    if (!has_vertex(v)) {
      return;
    }

    vertices = new char*[256];
    weights = new int*[256];

    for (auto it = edges_->cbegin(); it != edges_->cend(); ++it) {
      auto pair = *it;
      const std::pair< char*, char* >& edge_key = pair.first;
      EdgeList* el = pair.second;

      if (std::strcmp(edge_key.first, v) == 0) {
        vertices[count] = edge_key.second;
        weights[count] = el->weights;
        size_t w_count = el->count;
        int* sorted_weights = new int[w_count];
        for (size_t i = 0; i < w_count; ++i) {
          sorted_weights[i] = el->weights[i];
        }
        for (size_t i = 0; i < w_count - 1; ++i) {
          for (size_t j = 0; j < w_count - i - 1; ++j) {
            if (sorted_weights[j] > sorted_weights[j + 1]) {
              int tmp = sorted_weights[j];
              sorted_weights[j] = sorted_weights[j + 1];
              sorted_weights[j + 1] = tmp;
            }
          }
        }
        weights[count] = sorted_weights;
        count++;
      }
    }
  }

  void Graph::get_inbound(const char* v, char**& vertices, int**& weights, size_t& count) const {
    count = 0;
    vertices = nullptr;
    weights = nullptr;

    if (!has_vertex(v)) {
      return;
    }

    vertices = new char*[256];
    weights = new int*[256];

    for (auto it = edges_->cbegin(); it != edges_->cend(); ++it) {
      auto pair = *it;
      const std::pair< char*, char* >& edge_key = pair.first;
      EdgeList* el = pair.second;

      if (std::strcmp(edge_key.second, v) == 0) {
        vertices[count] = edge_key.first;
        size_t w_count = el->count;
        int* sorted_weights = new int[w_count];
        for (size_t i = 0; i < w_count; ++i) {
          sorted_weights[i] = el->weights[i];
        }
        for (size_t i = 0; i < w_count - 1; ++i) {
          for (size_t j = 0; j < w_count - i - 1; ++j) {
            if (sorted_weights[j] > sorted_weights[j + 1]) {
              int tmp = sorted_weights[j];
              sorted_weights[j] = sorted_weights[j + 1];
              sorted_weights[j + 1] = tmp;
            }
          }
        }
        weights[count] = sorted_weights;
        count++;
      }
    }
  }

}
