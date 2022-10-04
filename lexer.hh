#pragma once
#include "defs.hh"

#include <cassert>
#include <cctype>
#include <istream>
#include <memory>
#include <optional>
#include <string>
#include <iostream>

class LexicalAnalyzer {
public:
  explicit LexicalAnalyzer(std::shared_ptr<std::istream> source)
      : in{std::move(source)} {
    // source->unsetf(std::ios_base::skipws);
    nextChar();
  }

  void nextToken() {
    while (curChar && std::isspace(*curChar)) {
      nextChar();
    }
    if (!curChar) {
      curToken = {TokenType::END, ""};
      return;
    }
    switch (*curChar) {
    case '(':
      nextChar();
      curToken = {TokenType::LPAREN, "("};
      break;
    case ')':
      nextChar();
      curToken = {TokenType::RPAREN, ")"};
      break;
    case '+':
      nextChar();
      curToken = {TokenType::PLUS, "+"};
      break;
    case '*':
      nextChar();
      curToken = {TokenType::ASTERISK, "*"};
      break;
    case ':':
      nextChar();
      curToken = {TokenType::COLON, ":"};
      break;
    case ',':
      nextChar();
      curToken = {TokenType::COMMA, ","};
      break;
    default: {
      if (!isLetter(*curChar)) {
        throw ParserException(
            MyFormat("There are no tokens that start with `%` (at pos %)",
                     *curChar, curPos));
      }
      curToken.value.clear();
      while (curChar && isLetter(*curChar)) {
        curToken.value.push_back(*curChar);
        nextChar();
      }
      if (curToken.value == "lambda") {
        curToken.type = TokenType::LAMBDA_KW;
      } else {
        curToken.type = TokenType::VARIABLE;
      }
    }
    }
  }

  Token getToken() const {
    std::cout << "Looking at token `" << curToken.value << "` of type " << ToString(curToken.type) << std::endl;
    return curToken;
  }

  std::size_t getPos() const {
    return curPos;
  }

private:
  void nextChar() {
    curPos++;
    if (char c; *in >> c) {
      curChar.emplace(c);
    } else if (in->eof()) {
      curChar = std::nullopt;
      curToken = {TokenType::END, ""};
    } else {
      throw ParserException(MyFormat(
          "Error encountered while reading character at position %", curPos));
    }
  }

  static bool isLetter(char c) {
    return 'a' <= c && c <= 'z';
  }

private:
  /// NOTE: we use shared_ptr, because it has an ability to store a type-erased
  /// destructor - this can be useful if we want to work with std::cin (and we
  /// definetely want to)
  std::shared_ptr<std::istream> in;
  std::optional<char> curChar{};
  std::size_t curPos{0};
  Token curToken;
};
