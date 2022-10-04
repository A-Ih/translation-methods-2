#pragma once
#include <stdexcept>
#include <string>
#include <sstream>
#include <string_view>
#include <array>

/// A marker-exception that we can catch and process

class ParserException : std::runtime_error {
  using std::runtime_error::runtime_error;
};

enum class TokenType {
  LPAREN,
  RPAREN,
  END,
  PLUS,
  ASTERISK,
  VARIABLE,
  LAMBDA_KW,
  COLON,
  COMMA
};

inline const char* toString(TokenType type) {
  switch (type) {
  case TokenType::LPAREN:
    return "LPAREN";
  case TokenType::RPAREN:
    return "RPAREN";
  case TokenType::END:
    return "END";
  case TokenType::PLUS:
    return "PLUS";
  case TokenType::ASTERISK:
    return "ASTERISK";
  case TokenType::VARIABLE:
    return "VARIABLE";
  case TokenType::LAMBDA_KW:
    return "LAMBDA_KW";
  case TokenType::COLON:
    return "COLON";
  case TokenType::COMMA:
    return "COMMA";
  }
}

inline std::ostream& operator<<(std::ostream& os, const TokenType& type) {
  return os << toString(type);
}

/// Get Nth arg from parameter pack
template<std::size_t N, typename Head, typename ...Tail>
struct NthHelper {
  static auto get(Head, Tail... tail) {
    return NthHelper<N - 1, Tail...>(std::forward<Tail>(tail)...);
  }
};

template<typename Head, typename ...Tail>
struct NthHelper<0, Head, Tail...> {
  static Head get(Head head, Tail...) {
    return head;
  }
};

namespace detail {
  template <typename T>
    std::string to_string(const T& x) {
      std::ostringstream os;
      os << x;
      return os.str();
    }
}

/// Only the most recent compilers support <format> out of the box
/// So here's my improvised implementation
/// Replaces all `%` symbols with args
template <typename ...Args>
std::string my_format(std::string_view formatStr, Args&&...args) {
  if constexpr (sizeof...(Args) == 0) {
    return std::string{formatStr};
  }
  if (std::count(formatStr.begin(), formatStr.end(), '%') != sizeof...(Args)) {
    throw std::runtime_error(
        "Number of arguments doesn't match number of replacement spots");
  }
  auto replacements =
      { detail::to_string(args)...};
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
  TokenType type;
  /// For `$` value is equal to empty string
  std::string value;
};
