#pragma once
#include <array>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

/// A marker-exception that we can catch and process

class ParserException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

enum class TokenType {
  LPAREN,
  RPAREN,
  END,
  VBAR,
  AMPERSAND,
  TILDA,
  PLUS,
  ASTERISK,
  VARIABLE,
  CONSTANT,
  LAMBDA_KW,
  COLON,
  COMMA
};

inline const char* ToString(TokenType type) {
  switch (type) {
  case TokenType::LPAREN:
    return "LPAREN";
  case TokenType::RPAREN:
    return "RPAREN";
  case TokenType::END:
    return "END";
  case TokenType::VBAR:
    return "VBAR";
  case TokenType::AMPERSAND:
    return "AMPERSAND";
  case TokenType::TILDA:
    return "TILDA";
  case TokenType::PLUS:
    return "PLUS";
  case TokenType::ASTERISK:
    return "ASTERISK";
  case TokenType::VARIABLE:
    return "VARIABLE";
  case TokenType::CONSTANT:
    return "CONSTANT";
  case TokenType::LAMBDA_KW:
    return "LAMBDA_KW";
  case TokenType::COLON:
    return "COLON";
  case TokenType::COMMA:
    return "COMMA";
  }
}

inline std::ostream& operator<<(std::ostream& os, const TokenType& type) {
  return os << ToString(type);
}

namespace detail {
template <typename T>
std::string ToString(const T& x) {
  std::ostringstream os;
  os << x;
  return os.str();
}
} // namespace detail

/// Only the most recent compilers support <format> out of the box
/// So here's my improvised implementation
/// Replaces all `%` symbols with args
template <typename... Args>
std::string MyFormat(std::string_view formatStr, Args&&... args) {
  if constexpr (sizeof...(Args) == 0) {
    return std::string{formatStr};
  }
  if (std::count(formatStr.begin(), formatStr.end(), '%') != sizeof...(Args)) {
    throw std::runtime_error(
        "Number of arguments doesn't match number of replacement spots");
  }
  auto replacements = {detail::ToString(args)...};
  auto currentReplacement = replacements.begin();
  std::ostringstream result;
  for (auto c : formatStr) {
    if (c == '%') {
      result << *currentReplacement;
      currentReplacement++;
    } else {
      result << c;
    }
  }
  return result.str();
}

struct Token {
  TokenType type{TokenType::END};
  /// For `$` value is equal to empty string
  std::string value{};
};
