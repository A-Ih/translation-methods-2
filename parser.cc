#include "parser.hh"

#include <memory>

namespace {
void expect(const LexicalAnalyzer& lex, std::string_view expected) {
  auto [type, value] = lex.getToken();
  throw ParserException(
      MyFormat("Expected %, got `%` of type '%' at position %", expected, value,
               type, lex.getPos() - value.size()));
}
} // namespace

Parser::TPtr Parser::ParseToken(TokenType expectedType) {
  auto [type, value] = lex.getToken();
  if (type != expectedType) {
    throw ParserException(
        MyFormat("Expected token of type %, got `%` of type % at position %",
                 expectedType, value, type, lex.getPos() - value.size()));
  }
  lex.nextToken();
  return std::make_unique<Tree>(Tree{std::move(value), {}});
}

Parser::TPtr Parser::Declaration() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
  case TokenType::LAMBDA_KW: {
    children.emplace_back(ParseToken(TokenType::LAMBDA_KW));
    children.emplace_back(Arglist());
    children.emplace_back(ParseToken(TokenType::COLON));
    children.emplace_back(El());
    break;
  }
  default:
    expect(lex, "lambda declaration");
  }
  return std::make_unique<Tree>(Tree{"Declaration", std::move(children)});
}

Parser::TPtr Parser::Arglist() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
  case TokenType::VARIABLE: {
    children.emplace_back(Varlist());
    break;
  }
  case TokenType::COLON: {
    // noop
    // TokenType::Colon is from FOLLOW(Arglist)
    break;
  }
  default:
    expect(lex, "argument list");
  }
  return std::make_unique<Tree>(Tree{"Arglist", std::move(children)});
}

Parser::TPtr Parser::Varlist() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
  case TokenType::VARIABLE: {
    children.emplace_back(ParseToken(TokenType::VARIABLE));
    children.emplace_back(VarlistPrime());
    break;
  }
  default:
    expect(lex, "variable list");
  }
  return std::make_unique<Tree>(Tree{"Varlist", std::move(children)});
}

Parser::TPtr Parser::VarlistPrime() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
  case TokenType::COMMA: {
    children.emplace_back(ParseToken(TokenType::COMMA));
    children.emplace_back(ParseToken(TokenType::VARIABLE));
    children.emplace_back(VarlistPrime());
    break;
  }
  case TokenType::COLON: {
    // noop
    break;
  }
  default:
    expect(lex, "continuation of variable list");
  }
  return std::make_unique<Tree>(Tree{"Varlist'", std::move(children)});
}

Parser::TPtr Parser::El() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
  case TokenType::VARIABLE:
  case TokenType::CONSTANT:
  case TokenType::TILDA:
  case TokenType::LPAREN: {
    children.emplace_back(Tl());
    children.emplace_back(ElPrime());
    break;
  }
  default:
    expect(lex, "El");
  }
  return std::make_unique<Tree>(Tree{"El", std::move(children)});
}

Parser::TPtr Parser::ElPrime() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
  case TokenType::VBAR: {
    children.emplace_back(ParseToken(TokenType::VBAR));
    children.emplace_back(Tl());
    children.emplace_back(ElPrime());
    break;
  }
  case TokenType::END:
  case TokenType::RPAREN: {
    // noop
    break;
  }
  default:
    expect(lex, "El'");
  }
  return std::make_unique<Tree>(Tree{"El'", std::move(children)});
}

Parser::TPtr Parser::Tl() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
  case TokenType::VARIABLE:
  case TokenType::CONSTANT:
  case TokenType::LPAREN: {
    children.emplace_back(Fl());
    children.emplace_back(TlPrime());
    break;
  }
  case TokenType::TILDA: {
    children.emplace_back(ParseToken(TokenType::TILDA));
    children.emplace_back(Fl());
    children.emplace_back(TlPrime());
    break;
  }
  default:
    expect(lex, "Tl");
  }
  return std::make_unique<Tree>(Tree{"Tl", std::move(children)});
}

Parser::TPtr Parser::TlPrime() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
  case TokenType::AMPERSAND: {
    children.emplace_back(ParseToken(TokenType::AMPERSAND));
    children.emplace_back(Fl());
    children.emplace_back(TlPrime());
    break;
  }
  case TokenType::END:
  case TokenType::VBAR:
  case TokenType::RPAREN: {
    // noop
    break;
  }
  default:
    expect(lex, "Tl'");
  }
  return std::make_unique<Tree>(Tree{"Tl'", std::move(children)});
}

Parser::TPtr Parser::Fl() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
  case TokenType::VARIABLE:
  case TokenType::CONSTANT:
  case TokenType::LPAREN: {
    children.emplace_back(Expression());
    break;
  }
  case TokenType::TILDA: {
    children.emplace_back(ParseToken(TokenType::TILDA));
    children.emplace_back(Fl());
    break;
  }
  default:
    expect(lex, "Fl");
  }
  return std::make_unique<Tree>(Tree{"Fl", std::move(children)});
}

Parser::TPtr Parser::Expression() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
  case TokenType::VARIABLE:
  case TokenType::CONSTANT:
  case TokenType::LPAREN: {
    children.emplace_back(Term());
    children.emplace_back(ExpressionPrime());
    break;
  }
  default:
    expect(lex, "Expression");
  }
  return std::make_unique<Tree>(Tree{"Expression", std::move(children)});
}

Parser::TPtr Parser::ExpressionPrime() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
  case TokenType::PLUS: {
    children.emplace_back(ParseToken(TokenType::PLUS));
    children.emplace_back(Term());
    children.emplace_back(ExpressionPrime());
    break;
  }
  case TokenType::END:
  case TokenType::VBAR:
  case TokenType::AMPERSAND:
  case TokenType::RPAREN: {
    // noop
    break;
  }
  default:
    expect(lex, "Expression'");
  }
  return std::make_unique<Tree>(Tree{"Expression'", std::move(children)});
}

Parser::TPtr Parser::Term() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
  case TokenType::LPAREN:
  case TokenType::CONSTANT:
  case TokenType::VARIABLE: {
    children.emplace_back(Factor());
    children.emplace_back(TermPrime());
    break;
  }
  default:
    expect(lex, "Term");
  }
  return std::make_unique<Tree>(Tree{"Term", std::move(children)});
}

Parser::TPtr Parser::TermPrime() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
  case TokenType::ASTERISK: {
    children.emplace_back(ParseToken(TokenType::ASTERISK));
    children.emplace_back(Factor());
    children.emplace_back(TermPrime());
    break;
  }
  case TokenType::END:
  case TokenType::VBAR:
  case TokenType::AMPERSAND:
  case TokenType::PLUS:
  case TokenType::RPAREN: {
    // noop
    break;
  }
  default:
    expect(lex, "Term'");
  }
  return std::make_unique<Tree>(Tree{"Term'", std::move(children)});
}

Parser::TPtr Parser::Factor() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
  case TokenType::CONSTANT: {
    children.emplace_back(ParseToken(TokenType::CONSTANT));
    break;
  }
  case TokenType::VARIABLE: {
    children.emplace_back(ParseToken(TokenType::VARIABLE));
    break;
  }
  case TokenType::LPAREN: {
    children.emplace_back(ParseToken(TokenType::LPAREN));
    children.emplace_back(El());
    children.emplace_back(ParseToken(TokenType::RPAREN));
    break;
  }
  default:
    expect(lex, "Factor");
  }
  return std::make_unique<Tree>(Tree{"Factor", std::move(children)});
}
