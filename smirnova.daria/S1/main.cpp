#include <iostream>

#include "sequences.hpp"

int main()
{
  const smirnova::Sequences seqs = smirnova::readSequences();

  if (seqs.empty()) {
    std::cout << 0 << '\n';
    return 0;
  }

  smirnova::printNames(seqs);

  const smirnova::Transposed transposed = smirnova::buildTransposed(seqs);

  if (transposed.empty()) {
    std::cerr << "error: sum calculation is impossible\n";
    return 1;
  }

  for (smirnova::LCIter< smirnova::Numbers > it = transposed.cbegin();
       it != transposed.cend();
       ++it) {
    smirnova::printNumbers(*it);
  }

  smirnova::printSums(transposed);

  return 0;
}
