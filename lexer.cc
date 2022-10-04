#include "lexer.hh"

LexicalAnalyzer::LexicalAnalyzer(std::shared_ptr<std::istream> source)
    : in{std::move(source)} {
  // source->unsetf(std::ios_base::skipws);
  nextChar();
}

void LexicalAnalyzer::nextToken() {
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

Token LexicalAnalyzer::getToken() const {
  return curToken;
}

std::size_t LexicalAnalyzer::getPos() const {
  return curPos;
}

bool LexicalAnalyzer::isLetter(char c) {
  return 'a' <= c && c <= 'z';
}

void LexicalAnalyzer::nextChar() {
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
