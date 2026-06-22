#ifndef SMIRNOVA_BLAKE2_HPP
#define SMIRNOVA_BLAKE2_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace smirnova {

  class Blake2 {
  public:
    static constexpr size_t BLOCK_SIZE = 64;
    static constexpr size_t OUT_SIZE = 32;

    Blake2();
    void update(const uint8_t* data, size_t len);
    void finalize(uint8_t* out);
    void reset();

    size_t operator()(const char* str) const;

  private:
    uint64_t h_[8];
    uint8_t buf_[BLOCK_SIZE];
    uint64_t counter_;
    size_t buf_len_;

    static const uint64_t IV[8];
    static const uint8_t SIGMA[12][16];

    void process_block(const uint8_t* block);
    uint64_t rotr64(uint64_t x, int n) const;
    void mix(uint64_t& a, uint64_t& b, uint64_t& c, uint64_t& d, uint64_t x, uint64_t y);
  };

}

#endif
