#pragma once

#include "defs.hh"
#include "lexer.hh"
#include "tree.hh"

#include <memory>
#include <sstream>
#include <string>

class Parser {
public:
  using TPtr = std::unique_ptr<Tree>;
  explicit Parser(std::string stringSource)
      : lex{std::make_shared<std::istringstream>(std::move(stringSource))} {}

  explicit Parser(std::shared_ptr<std::istream> streamSource)
      : lex{streamSource} {}

  TPtr Parse() {
    lex.nextToken();
    return Declaration();
  }

  TPtr ParseToken(TokenType expectedType);

  TPtr Declaration();
  TPtr Arglist();
  TPtr Varlist();
  TPtr VarlistPrime();
  TPtr El();
  TPtr ElPrime();
  TPtr Tl();
  TPtr TlPrime();
  TPtr Fl();
  TPtr Expression();
  TPtr ExpressionPrime();
  TPtr Term();
  TPtr TermPrime();
  TPtr Factor();

private:
  LexicalAnalyzer lex;
};
