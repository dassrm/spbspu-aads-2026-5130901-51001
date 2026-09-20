#ifndef SMIRNOVA_STRING_HASH_HPP
#define SMIRNOVA_STRING_HASH_HPP

#include <cstddef>
#include <string>

namespace smirnova {

  // Hand-written FNV-1a hash, used as the default hasher for HashTable
  // whenever the key is a std::string, so the hashing itself is our own
  // code rather than std::hash<std::string>.
  struct StringHash {
    std::size_t operator()(const std::string& key) const {
      std::size_t hash = 14695981039346656037ull;
      for (unsigned char symbol : key) {
        hash ^= symbol;
        hash *= 1099511628211ull;
      }
      return hash;
    }
  };

}

#endif
