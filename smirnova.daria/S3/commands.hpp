#ifndef SMIRNOVA_COMMANDS_HPP
#define SMIRNOVA_COMMANDS_HPP

#include "graph.hpp"

namespace smirnova {

  class GraphManager {
  public:
    GraphManager();
    ~GraphManager();

    void add_graph(const char* name, Graph* graph);
    Graph* get_graph(const char* name);
    bool has_graph(const char* name) const;
    void execute_command(const char* command);
    void list_graphs() const;

  private:
    HashTable< char*, Graph*, StringHash, StringEqual >* graphs_;

    char* copy_string(const char* str);
    void merge_graphs(const char* result, const char* g1, const char* g2);
    void extract_vertices(const char* result, const char* graph, char** vertices, size_t count);
  };

}

#endif
