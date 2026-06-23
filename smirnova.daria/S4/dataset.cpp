#include "dataset.hpp"
#include <cstring>
#include <cstdlib>

namespace smirnova {

  DatasetManager::DatasetManager() : capacity_(10), size_(0) {
    datasets_ = static_cast< DatasetStorage* >(::operator new(capacity_ * sizeof(DatasetStorage)));
  }

  DatasetManager::~DatasetManager() {
    for (size_t i = 0; i < size_; ++i) {
      delete datasets_[i].tree;
      delete[] datasets_[i].name;
    }
    ::operator delete(datasets_);
  }

  BSTree< char*, char* >* DatasetManager::getDataset(const char* name) {
    for (size_t i = 0; i < size_; ++i) {
      if (std::strcmp(datasets_[i].name, name) == 0) {
        return datasets_[i].tree;
      }
    }
    return nullptr;
  }

  void DatasetManager::createDataset(const char* name) {
    if (datasetExists(name)) {
      return;
    }
    if (size_ >= capacity_) {
      resize();
    }
    datasets_[size_].name = new char[std::strlen(name) + 1];
    std::strcpy(datasets_[size_].name, name);
    datasets_[size_].tree = new BSTree< char*, char* >();
    size_++;
  }

  void DatasetManager::deleteDataset(const char* name) {
    for (size_t i = 0; i < size_; ++i) {
      if (std::strcmp(datasets_[i].name, name) == 0) {
        delete datasets_[i].tree;
        delete[] datasets_[i].name;
        for (size_t j = i; j < size_ - 1; ++j) {
          datasets_[j] = datasets_[j + 1];
        }
        size_--;
        return;
      }
    }
  }

  bool DatasetManager::datasetExists(const char* name) const {
    for (size_t i = 0; i < size_; ++i) {
      if (std::strcmp(datasets_[i].name, name) == 0) {
        return true;
      }
    }
    return false;
  }

  size_t DatasetManager::getDatasetCount() const {
    return size_;
  }

  void DatasetManager::resize() {
    capacity_ *= 2;
    DatasetStorage* newStorage = static_cast< DatasetStorage* >(::operator new(capacity_ * sizeof(DatasetStorage)));
    for (size_t i = 0; i < size_; ++i) {
      newStorage[i] = datasets_[i];
    }
    ::operator delete(datasets_);
    datasets_ = newStorage;
  }

}
