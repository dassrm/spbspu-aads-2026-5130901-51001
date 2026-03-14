#include "sequences.hpp"

#include <iostream>
#include <string>

smirnova::Sequences smirnova::readSequences()
{
  Sequences seqs;
  std::string name;
  while (std::cin >> name) {
    Numbers nums;
    long long num = 0;
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

void smirnova::printNames(const smirnova::Sequences& seqs)
{
  bool first = true;
  for (LCIter< NamedSeq > it = seqs.cbegin(); it != seqs.cend(); ++it) {
    if (!first) {
      std::cout << ' ';
    }
    std::cout << it->first;
    first = false;
  }
  std::cout << '\n';
}

void smirnova::printNumbers(const smirnova::Numbers& nums)
{
  bool first = true;
  for (LCIter< long long > it = nums.cbegin(); it != nums.cend(); ++it) {
    if (!first) {
      std::cout << ' ';
    }
    std::cout << *it;
    first = false;
  }
  std::cout << '\n';
}

smirnova::Transposed smirnova::buildTransposed(const smirnova::Sequences& seqs)
{
  List< LCIter< long long > > iters;
  for (LCIter< NamedSeq > it = seqs.cbegin(); it != seqs.cend(); ++it) {
    iters.pushBack(it->second.cbegin());
  }

  Transposed transposed;
  while (true) {
    Numbers row;
    LCIter< NamedSeq > seqIt = seqs.cbegin();
    LIter< LCIter< long long > > iterIt = iters.begin();
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

long long smirnova::computeSum(const smirnova::Numbers& nums)
{
  long long sum = 0;
  for (LCIter< long long > it = nums.cbegin(); it != nums.cend(); ++it) {
    sum += *it;
  }
  return sum;
}

void smirnova::printSums(const smirnova::Transposed& transposed)
{
  bool first = true;
  for (LCIter< Numbers > it = transposed.cbegin(); it != transposed.cend(); ++it) {
    if (!first) {
      std::cout << ' ';
    }
    std::cout << computeSum(*it);
    first = false;
  }
  std::cout << '\n';
}
