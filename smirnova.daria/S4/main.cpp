#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace smirnova;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << argv[1] << "\n";
    return 1;
  }

  DatasetManager manager;
  const size_t bufferSize = 4096;
  char* line = new char[bufferSize];

  while (file.getline(line, bufferSize)) {
    if (*line == '\0' || *line == '#') {
      continue;
    }

    char* lineCopy = new char[std::strlen(line) + 1];
    std::strcpy(lineCopy, line);

    char* datasetName = std::strtok(lineCopy, " ");
    if (datasetName == nullptr) {
      delete[] lineCopy;
      continue;
    }

    manager.createDataset(datasetName);
    BSTree< char*, char* >* tree = manager.getDataset(datasetName);

    char* key = std::strtok(nullptr, " ");
    while (key != nullptr) {
      char* value = std::strtok(nullptr, " ");
      if (value == nullptr) {
        break;
      }
      
      char* keyStr = new char[std::strlen(key) + 1];
      char* valueStr = new char[std::strlen(value) + 1];
      std::strcpy(keyStr, key);
      std::strcpy(valueStr, value);
      
      tree->push(keyStr, valueStr);

      key = std::strtok(nullptr, " ");
    }

    delete[] lineCopy;
  }

  file.close();

  std::cout << "Enter commands (Ctrl+D on Linux or Ctrl+Z on Windows to exit):\n";
  while (std::cin.getline(line, bufferSize)) {
    if (*line == '\0') {
      continue;
    }
    executeCommand(line, manager);
  }

  delete[] line;
  return 0;
}
