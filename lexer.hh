#pragma once
#include "defs.hh"

#include <cctype>
#include <istream>
#include <memory>
#include <optional>
#include <string>

class LexicalAnalyzer {
public:
  explicit LexicalAnalyzer(std::shared_ptr<std::istream> source);

  void nextToken();
  Token getToken() const;
  std::size_t getPos() const;

private:
  void nextChar();
  static bool isLetter(char c);
  static bool isDigit(char c);

private:
  /// NOTE: we use shared_ptr, because it has an ability to store a type-erased
  /// destructor - this can be useful if we want to work with std::cin (and we
  /// definetely want to)
  std::shared_ptr<std::istream> in;
  std::optional<char> curChar{};
  std::size_t curPos{0};
  Token curToken;
};
