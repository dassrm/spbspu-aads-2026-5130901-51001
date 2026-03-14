#include "sequences.hpp"
#include <iostream>
#include <string>
#include <climits>
#include <cstdlib>

smirnova::Sequences smirnova::readSequences()
{
  Sequences seqs;
  std::string name;
  while (std::cin >> name) {
    Numbers nums;
    long long num = 0;
    while (std::cin >> num) {
      // Проверка на переполнение при чтении
      if (num > LLONG_MAX || num < LLONG_MIN) {
        throw std::overflow_error("overflow in number reading");
      }
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
    bool hasData = false;
    
    while (seqIt != seqs.cend()) {
      if (*iterIt != seqIt->second.cend()) {
        long long val = **iterIt;
        // Проверка на переполнение при чтении
        if (val > LLONG_MAX || val < LLONG_MIN) {
          throw std::overflow_error("overflow in number reading");
        }
        row.pushBack(val);
        ++(*iterIt);
        hasData = true;
      }
      ++seqIt;
      ++iterIt;
    }
    
    if (!hasData) {
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
    // Проверка на переполнение при сложении
    if (*it > 0 && sum > LLONG_MAX - *it) {
      throw std::overflow_error("overflow in sum calculation");
    }
    if (*it < 0 && sum < LLONG_MIN - *it) {
      throw std::overflow_error("overflow in sum calculation");
    }
    sum += *it;
  }
  return sum;
}

void smirnova::printSums(const smirnova::Transposed& transposed)
{
  if (transposed.empty()) {
    return;
  }

  bool first = true;
  for (LCIter< Numbers > it = transposed.cbegin(); it != transposed.cend(); ++it) {
    if (!first) {
      std::cout << ' ';
    }
    try {
      std::cout << computeSum(*it);
    } catch (const std::overflow_error&) {
      std::cerr << "error: overflow in sum calculation\n";
      std::exit(1);
    }
    first = false;
  }
  std::cout << '\n';
}
