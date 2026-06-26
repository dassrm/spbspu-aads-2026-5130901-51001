#include <climits>
#include <iostream>
#include <string>

#include "sequences.hpp"

namespace {

smirnova::Sequences readSequences()
{
  smirnova::Sequences seqs;
  std::string name;
  while (std::cin >> name) {
    smirnova::Numbers nums;
    unsigned long long num = 0;
    while (std::cin >> num) {
      nums.pushBack(num);
    }
    if (!std::cin.eof()) {
      std::cin.clear();
    }
    seqs.pushBack(std::make_pair(name, std::move(nums)));
  }
  return seqs;
}

void printNames(const smirnova::Sequences& seqs)
{
  bool first = true;
  for (smirnova::LCIter< smirnova::NamedSeq > it = seqs.cbegin(); it != seqs.cend(); ++it) {
    if (!first) {
      std::cout << ' ';
    }
    std::cout << it->first;
    first = false;
  }
  std::cout << '\n';
}

void printNumbers(const smirnova::Numbers& nums)
{
  bool first = true;
  for (smirnova::LCIter< unsigned long long > it = nums.cbegin(); it != nums.cend(); ++it) {
    if (!first) {
      std::cout << ' ';
    }
    std::cout << *it;
    first = false;
  }
  std::cout << '\n';
}

smirnova::Transposed buildTransposed(const smirnova::Sequences& seqs)
{
  smirnova::List< smirnova::LCIter< unsigned long long > > iters;
  for (smirnova::LCIter< smirnova::NamedSeq > it = seqs.cbegin(); it != seqs.cend(); ++it) {
    iters.pushBack(it->second.cbegin());
  }
  smirnova::Transposed transposed;
  while (true) {
    smirnova::Numbers row;
    smirnova::LCIter< smirnova::NamedSeq > seqIt = seqs.cbegin();
    smirnova::LIter< smirnova::LCIter< unsigned long long > > iterIt = iters.begin();
    while (seqIt != seqs.cend()) {
      if (*iterIt != seqIt->second.cend()) {
        row.pushBack(**iterIt);
        ++(*iterIt);
      }
      ++seqIt;
      ++iterIt;
    }
    if (row.empty()) {
      break;
    }
    transposed.pushBack(std::move(row));
  }
  return transposed;
}

bool computeSums(const smirnova::Transposed& transposed, smirnova::Numbers& sums)
{
  for (smirnova::LCIter< smirnova::Numbers > rowIt = transposed.cbegin();
       rowIt != transposed.cend();
       ++rowIt) {
    unsigned long long sum = 0;
    for (smirnova::LCIter< unsigned long long > it = rowIt->cbegin();
         it != rowIt->cend();
         ++it) {
      if (sum > ULLONG_MAX - *it) {
        return false;
      }
      sum += *it;
    }
    sums.pushBack(sum);
  }
  return true;
}

void printSums(const smirnova::Numbers& sums)
{
  bool first = true;
  for (smirnova::LCIter< unsigned long long > it = sums.cbegin(); it != sums.cend(); ++it) {
    if (!first) {
      std::cout << ' ';
    }
    std::cout << *it;
    first = false;
  }
  std::cout << '\n';
}

}

int main()
{
  const smirnova::Sequences seqs = readSequences();

  if (seqs.empty()) {
    std::cout << 0 << '\n';
    return 0;
  }

  printNames(seqs);

  const smirnova::Transposed transposed = buildTransposed(seqs);

  if (transposed.empty()) {
    std::cout << 0 << '\n';
    return 0;
  }

  for (smirnova::LCIter< smirnova::Numbers > it = transposed.cbegin();
       it != transposed.cend();
       ++it) {
    printNumbers(*it);
  }

  smirnova::Numbers sums;
  if (!computeSums(transposed, sums)) {
    std::cerr << "error: overflow in sum calculation\n";
    return 1;
  }

  printSums(sums);

  return 0;
}
