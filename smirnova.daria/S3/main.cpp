#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace smirnova;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Error opening file\n";
    return 1;
  }

  GraphManager manager;
  const size_t BUFFER_SIZE = 4096;
  char* line = new char[BUFFER_SIZE];

  while (file.getline(line, BUFFER_SIZE)) {
    if (*line == '\0' || *line == '#') {
      continue;
    }

    char* line_copy = new char[std::strlen(line) + 1];
    std::strcpy(line_copy, line);

    char* graph_name = std::strtok(line_copy, " ");
    if (!graph_name) {
      delete[] line_copy;
      continue;
    }

    char* edge_count_str = std::strtok(nullptr, " ");
    if (!edge_count_str) {
      delete[] line_copy;
      continue;
    }

    int edge_count = std::atoi(edge_count_str);
    Graph* graph = new Graph(graph_name);
    manager.add_graph(graph_name, graph);

    for (int i = 0; i < edge_count; ++i) {
      if (!file.getline(line, BUFFER_SIZE)) {
        break;
      }

      if (*line == '\0' || *line == '#') {
        i--;
        continue;
      }

      char* line_copy2 = new char[std::strlen(line) + 1];
      std::strcpy(line_copy2, line);

      char* from = std::strtok(line_copy2, " ");
      char* to = std::strtok(nullptr, " ");
      char* weight_str = std::strtok(nullptr, " ");

      if (from && to && weight_str) {
        int weight = std::atoi(weight_str);
        graph->add_edge(from, to, weight);
      }

      delete[] line_copy2;
    }

    delete[] line_copy;
  }

  file.close();

  std::cout << "Enter commands:\n";
  while (std::cin.getline(line, BUFFER_SIZE)) {
    manager.execute_command(line);
  }

  delete[] line;
  return 0;
}
