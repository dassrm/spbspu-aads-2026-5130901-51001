#include <fstream>
#include <iostream>
#include <string>

#include "eval.hpp"
#include "stack.hpp"

int main(int argc, char* argv[])
{
  std::istream* input = &std::cin;
  std::ifstream file;

  if (argc > 1) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "error: cannot open file: " << argv[1] << '\n';
      return 1;
    }
    input = &file;
  }

  smirnova::Stack< long long > results;
  std::string line;

  while (std::getline(*input, line)) {
    if (line.empty()) {
      continue;
    }
    try {
      results.push(smirnova::evalExpression(line));
    } catch (const std::exception& e) {
      std::cerr << "error: " << e.what() << '\n';
      return 1;
    }
  }

  bool first = true;
  while (!results.empty()) {
    if (!first) {
      std::cout << ' ';
    }
    std::cout << results.drop();
    first = false;
  }
  if (!first) {
    std::cout << '\n';
  }

  return 0;
}
