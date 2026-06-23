#include "commands.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>

namespace smirnova {

  void printDataset(const char* name, DatasetManager& manager) {
    BSTree< char*, char* >* tree = manager.getDataset(name);
    if (tree == nullptr) {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    for (auto it = tree->begin(); it != tree->end(); ++it) {
      auto pair = *it;
      std::cout << pair.first.get() << " " << pair.second.get() << " ";
    }
    std::cout << "\n";
  }

  void complementDataset(const char* result, const char* ds1, const char* ds2, DatasetManager& manager) {
    BSTree< char*, char* >* tree1 = manager.getDataset(ds1);
    BSTree< char*, char* >* tree2 = manager.getDataset(ds2);
    if (tree1 == nullptr || tree2 == nullptr) {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    manager.createDataset(result);
    BSTree< char*, char* >* resultTree = manager.getDataset(result);
    
    for (auto it = tree1->begin(); it != tree1->end(); ++it) {
      auto pair = *it;
      if (tree2->get(pair.first.get()).empty()) {
        resultTree->push(pair.first.get(), pair.second.get());
      }
    }
  }

  void intersectDataset(const char* result, const char* ds1, const char* ds2, DatasetManager& manager) {
    BSTree< char*, char* >* tree1 = manager.getDataset(ds1);
    BSTree< char*, char* >* tree2 = manager.getDataset(ds2);
    if (tree1 == nullptr || tree2 == nullptr) {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    manager.createDataset(result);
    BSTree< char*, char* >* resultTree = manager.getDataset(result);
    
    for (auto it = tree1->begin(); it != tree1->end(); ++it) {
      auto pair = *it;
      if (!tree2->get(pair.first.get()).empty()) {
        resultTree->push(pair.first.get(), pair.second.get());
      }
    }
  }

  void unionDataset(const char* result, const char* ds1, const char* ds2, DatasetManager& manager) {
    BSTree< char*, char* >* tree1 = manager.getDataset(ds1);
    BSTree< char*, char* >* tree2 = manager.getDataset(ds2);
    if (tree1 == nullptr || tree2 == nullptr) {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    manager.createDataset(result);
    BSTree< char*, char* >* resultTree = manager.getDataset(result);
    
    for (auto it = tree1->begin(); it != tree1->end(); ++it) {
      auto pair = *it;
      resultTree->push(pair.first.get(), pair.second.get());
    }
    for (auto it = tree2->begin(); it != tree2->end(); ++it) {
      auto pair = *it;
      resultTree->push(pair.first.get(), pair.second.get());
    }
  }

  void executeCommand(const char* command, DatasetManager& manager) {
    if (command == nullptr || *command == '\0') {
      return;
    }
    
    // Parse command
    char* cmd = new char[std::strlen(command) + 1];
    std::strcpy(cmd, command);
    
    // Simple tokenization
    char* token = std::strtok(cmd, " ");
    if (token == nullptr) {
      delete[] cmd;
      return;
    }
    
    if (std::strcmp(token, "print") == 0) {
      token = std::strtok(nullptr, " ");
      if (token != nullptr) {
        printDataset(token, manager);
      }
    } else if (std::strcmp(token, "complement") == 0) {
      char* result = std::strtok(nullptr, " ");
      char* ds1 = std::strtok(nullptr, " ");
      char* ds2 = std::strtok(nullptr, " ");
      if (result != nullptr && ds1 != nullptr && ds2 != nullptr) {
        complementDataset(result, ds1, ds2, manager);
      } else {
        std::cout << "<INVALID COMMAND>\n";
      }
    } else if (std::strcmp(token, "intersect") == 0) {
      char* result = std::strtok(nullptr, " ");
      char* ds1 = std::strtok(nullptr, " ");
      char* ds2 = std::strtok(nullptr, " ");
      if (result != nullptr && ds1 != nullptr && ds2 != nullptr) {
        intersectDataset(result, ds1, ds2, manager);
      } else {
        std::cout << "<INVALID COMMAND>\n";
      }
    } else if (std::strcmp(token, "union") == 0) {
      char* result = std::strtok(nullptr, " ");
      char* ds1 = std::strtok(nullptr, " ");
      char* ds2 = std::strtok(nullptr, " ");
      if (result != nullptr && ds1 != nullptr && ds2 != nullptr) {
        unionDataset(result, ds1, ds2, manager);
      } else {
        std::cout << "<INVALID COMMAND>\n";
      }
    } else {
      std::cout << "<INVALID COMMAND>\n";
    }
    
    delete[] cmd;
  }

}
