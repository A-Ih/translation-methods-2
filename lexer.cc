#include "lexer.hh"

LexicalAnalyzer::LexicalAnalyzer(std::shared_ptr<std::istream> source)
    : in{std::move(source)} {
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
  case '|':
    nextChar();
    curToken = {TokenType::VBAR, "|"};
    break;
  case '&':
    nextChar();
    curToken = {TokenType::AMPERSAND, "&"};
    break;
  case '~':
    nextChar();
    curToken = {TokenType::TILDA, "~"};
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
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9': {
    curToken.value.clear();
    while (curChar && isDigit(*curChar)) {
      curToken.value.push_back(*curChar);
      nextChar();
    }
    curToken.type = TokenType::CONSTANT;
    break;
  }
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

bool LexicalAnalyzer::isDigit(char c) {
  return '0' <= c && c <= '9';
}

void LexicalAnalyzer::nextChar() {
  if (char c; in && *in >> c) {
    curPos++;
    curChar.emplace(c);
  } else if (in->eof()) {
    curChar = std::nullopt;
    in.reset();
  } else {
    throw ParserException(MyFormat(
        "Error encountered while reading character at position %", curPos));
  }
}
