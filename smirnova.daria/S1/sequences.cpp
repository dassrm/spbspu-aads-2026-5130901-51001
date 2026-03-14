#include "sequences.hpp"
#include <iostream>
#include <string>
#include <climits>
#include <cstdlib>
#include <cerrno>
#include <cctype>

smirnova::Sequences smirnova::readSequences()
{
  Sequences seqs;
  std::string name;
  std::string token;

  while (std::cin >> name) {
    Numbers nums;

    while (std::cin >> token) {
      bool isNumber = true;
      size_t start = (token[0] == '-') ? 1 : 0;

      if (start == 1 && token.length() == 1) {
        isNumber = false;
      } else {
        for (size_t i = start; i < token.length(); ++i) {
          if (!isdigit(token[i])) {
            isNumber = false;
            break;
          }
        }
      }

      if (!isNumber) {
        for (int i = token.length() - 1; i >= 0; --i) {
          std::cin.putback(token[i]);
        }
        break;
      }

      char* endptr;
      errno = 0;
      long long num = std::strtoll(token.c_str(), &endptr, 10);

      if (errno == ERANGE || *endptr != '\0') {
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
    long long sum = computeSum(*it);
    std::cout << sum;
    first = false;
  }
  std::cout << '\n';
}
