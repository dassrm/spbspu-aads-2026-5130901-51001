#ifndef SMIRNOVA_DATASET_HPP
#define SMIRNOVA_DATASET_HPP

#include "bstree.hpp"
#include <cstddef>

namespace smirnova {

  struct DatasetStorage {
    char name[256];
    BSTree< char*, char* >* tree;
  };

  class DatasetManager {
  public:
    DatasetManager();
    ~DatasetManager();

    BSTree< char*, char* >* getDataset(const char* name);
    void createDataset(const char* name);
    void deleteDataset(const char* name);
    bool datasetExists(const char* name) const;
    size_t getDatasetCount() const;

  private:
    DatasetStorage* datasets_;
    size_t capacity_;
    size_t size_;

    void resize();
  };

}

#endif
