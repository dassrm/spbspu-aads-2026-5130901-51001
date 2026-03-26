#include "eval.hpp"

#include <stdexcept>
#include <string>

#include "queue.hpp"
#include "stack.hpp"

namespace {

int precedence(const std::string& tok)
{
  if (tok.size() != 1) {
    return 0;
  }
  switch (tok[0]) {
  case '*': case '/': case '%':
    return 3;
  case '+': case '-':
    return 2;
  case '&':
    return 1;
  default:
    return 0;
  }
}

long long applyOp(const std::string& op, long long a, long long b)
{
  switch (op[0]) {
  case '+': return a + b;
  case '-': return a - b;
  case '*': return a * b;
  case '/':
    if (b == 0) {
      throw std::runtime_error("division by zero");
    }
    return a / b;
  case '%':
    if (b == 0) {
      throw std::runtime_error("modulo by zero");
    }
    return a % b;
  case '&': return a & b;
  default:
    throw std::runtime_error("unknown operator: " + op);
  }
}

void processToken(
  const std::string& tok,
  smirnova::Queue< std::string >& output,
  smirnova::Stack< std::string >& ops)
{
  if (tok == "(") {
    ops.push(tok);
  } else if (tok == ")") {
    while (!ops.empty() && ops.top() != "(") {
      output.push(ops.drop());
    }
    if (ops.empty()) {
      throw std::runtime_error("mismatched parentheses");
    }
    ops.drop();
  } else if (precedence(tok) > 0) {
    while (!ops.empty() && precedence(ops.top()) >= precedence(tok)) {
      output.push(ops.drop());
    }
    ops.push(tok);
  } else {
    output.push(tok);
  }
}

}

namespace smirnova {

long long evalExpression(const std::string& line)
{
  Queue< std::string > output;
  Stack< std::string > ops;

  for (std::size_t i = 0, j = 0; j <= line.size(); ++j) {
    if (j == line.size() || line[j] == ' ') {
      if (j > i) {
        processToken(line.substr(i, j - i), output, ops);
      }
      i = j + 1;
    }
  }

  while (!ops.empty()) {
    const std::string op = ops.drop();
    if (op == "(" || op == ")") {
      throw std::runtime_error("mismatched parentheses");
    }
    output.push(op);
  }

  Stack< long long > vals;
  while (!output.empty()) {
    const std::string tok = output.drop();
    if (precedence(tok) > 0) {
      if (vals.size() < 2) {
        throw std::runtime_error("not enough operands for: " + tok);
      }
      const long long b = vals.drop();
      const long long a = vals.drop();
      vals.push(applyOp(tok, a, b));
    } else {
      std::size_t pos = 0;
      const long long num = std::stoll(tok, &pos);
      if (pos != tok.size()) {
        throw std::runtime_error("invalid token: " + tok);
      }
      vals.push(num);
    }
  }

  if (vals.empty()) {
    throw std::runtime_error("empty expression");
  }
  const long long result = vals.drop();
  if (!vals.empty()) {
    throw std::runtime_error("invalid expression: leftover values");
  }
  return result;
}

}
