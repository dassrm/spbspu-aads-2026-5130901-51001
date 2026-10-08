#ifndef SMIRNOVA_STRING_HASH_HPP
#define SMIRNOVA_STRING_HASH_HPP

#include <cstddef>
#include <cstdint>
#include <string>

namespace smirnova {

  struct StringHash {
    std::size_t operator()(const std::string& key) const
    {
      const std::uint64_t offsetBasis = 14695981039346656037ull;
      const std::uint64_t prime = 1099511628211ull;
      std::uint64_t hash = offsetBasis;
      for (unsigned char symbol : key) {
        hash ^= symbol;
        hash *= prime;
      }
      return static_cast< std::size_t >(hash);
    }
  };

}

#endif
