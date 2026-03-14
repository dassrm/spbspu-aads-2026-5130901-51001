#include <iostream>
#include <climits>

#include "sequences.hpp"

int main()
{
  const smirnova::Sequences seqs = smirnova::readSequences();

  if (seqs.empty()) {
    std::cout << 0 << '\n';
    return 0;
  }

  smirnova::printNames(seqs);

  try {
    const smirnova::Transposed transposed = smirnova::buildTransposed(seqs);

    if (transposed.empty()) {
      std::cout << 0 << '\n';
      return 0;
    }

    for (smirnova::LCIter< smirnova::Numbers > it = transposed.cbegin();
         it != transposed.cend();
         ++it) {
      smirnova::printNumbers(*it);
    }

    smirnova::printSums(transposed);
  } catch (const std::overflow_error&) {
    std::cerr << "error: overflow in sum calculation\n";
    return 1;
  }

  return 0;
}
