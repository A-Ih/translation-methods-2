#include "parser.hh"

#include <memory>

namespace {
void expect(const LexicalAnalyzer& lex, TokenType expected) {
  auto [type, value] = lex.getToken();
  if (type != expected) {
    throw ParserException(
        my_format("Expected token of type %, got % of type % at position %", toString(expected), value, type, lex.getPos()));
  }
}

void expect(const LexicalAnalyzer& lex, std::string_view expected) {
  auto [type, value] = lex.getToken();
    throw ParserException(
        my_format("Expected %, got % at position %", expected, value, lex.getPos()));
}
} // namespace

Parser::TPtr Parser::ParseToken(TokenType expectedType) {
  expect(lex, expectedType);
  auto [type, value] = lex.getToken();
  lex.nextToken();
  return std::make_unique<Tree>(Tree{std::move(value), {}});
}

Parser::TPtr Parser::Declaration() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
    case TokenType::LAMBDA_KW:
      {
        children.emplace_back(ParseToken(TokenType::LAMBDA_KW));
        children.emplace_back(Arglist());
        children.emplace_back(ParseToken(TokenType::COLON));
        children.emplace_back(Expression());
        break;
      }
    default:
      expect(lex, TokenType::LAMBDA_KW);
  }
  return std::make_unique<Tree>(Tree{"Declaration", std::move(children)});
}

Parser::TPtr Parser::Arglist() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
    case TokenType::VARIABLE:
      {
        children.emplace_back(Varlist());
        break;
      }
    case TokenType::COLON:
      {
        // noop
        // TokenType::Colon is from FOLLOW(Arglist)
        break;
      }
    default:
      throw ParserException(
          my_format("Expected argument list, got % on position %",
                    lex.getToken().value, lex.getPos()));
  }
  return std::make_unique<Tree>(Tree{"Arglist", std::move(children)});
}

Parser::TPtr Parser::Varlist() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
    case TokenType::VARIABLE:
      {
        children.emplace_back(ParseToken(TokenType::VARIABLE));
        children.emplace_back(VarlistPrime());
        break;
      }
    default:
      expect(lex, TokenType::VARIABLE);
  }
  return std::make_unique<Tree>(Tree{"Varlist", std::move(children)});
}

Parser::TPtr Parser::VarlistPrime() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
    case TokenType::COMMA:
      {
        children.emplace_back(ParseToken(TokenType::COMMA));
        children.emplace_back(ParseToken(TokenType::VARIABLE));
        children.emplace_back(VarlistPrime());
        break;
      }
    case TokenType::COLON:
      {
        // noop
        break;
      }
    default:
      throw ParserException(my_format(
          "Expected continuation of variable list, got % on position %",
          lex.getToken().value, lex.getPos()));
  }
  return std::make_unique<Tree>(Tree{"Varlist'", std::move(children)});
}

Parser::TPtr Parser::Expression() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
    case TokenType::VARIABLE:
    case TokenType::LPAREN:
      {
        children.emplace_back(Term());
        children.emplace_back(ExpressionPrime());
        break;
      }
    default:
      throw ParserException(my_format(
          "Expected beginning of expression, got % on position %",
          lex.getToken().value, lex.getPos()));
  }
  return std::make_unique<Tree>(Tree{"Expression", std::move(children)});
}

Parser::TPtr Parser::ExpressionPrime() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
    case TokenType::PLUS:
      {
        children.emplace_back(ParseToken(TokenType::PLUS));
        children.emplace_back(Term());
        children.emplace_back(ExpressionPrime());
        break;
      }
    case TokenType::END:
    case TokenType::RPAREN:
      {
        // noop
        break;
      }
    default:
      throw ParserException(my_format(
          "Expected beginning of expression, got % on position %",
          lex.getToken().value, lex.getPos()));
  }
  return std::make_unique<Tree>(Tree{"Expression'", std::move(children)});
}

Parser::TPtr Parser::Term() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
    case TokenType::LPAREN:
    case TokenType::VARIABLE:
      {
        children.emplace_back(Factor());
        children.emplace_back(TermPrime());
        break;
      }
    default:
      throw ParserException(my_format(
          "Expected beginning of term, got % on position %",
          lex.getToken().value, lex.getPos()));
  }
  return std::make_unique<Tree>(Tree{"Term", std::move(children)});
}

Parser::TPtr Parser::TermPrime() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
    case TokenType::ASTERISK:
      {
        children.emplace_back(ParseToken(TokenType::PLUS));
        children.emplace_back(Factor());
        children.emplace_back(TermPrime());
        break;
      }
    case TokenType::END:
    case TokenType::PLUS:
    case TokenType::RPAREN:
      {
        // noop
        break;
      }
    default:
      throw ParserException(my_format(
          "Expected continuation of term, got % on position %",
          lex.getToken().value, lex.getPos()));
  }
  return std::make_unique<Tree>(Tree{"Term'", std::move(children)});
}

Parser::TPtr Parser::Factor() {
  std::vector<TPtr> children;
  switch (auto [type, value] = lex.getToken(); type) {
    case TokenType::VARIABLE:
      {
        children.emplace_back(ParseToken(TokenType::VARIABLE));
        break;
      }
    case TokenType::LPAREN:
      {
        children.emplace_back(ParseToken(TokenType::LPAREN));
        break;
      }
    default:
      throw ParserException(my_format(
          "Expected beginning of factor, got % on position %",
          lex.getToken().value, lex.getPos()));
  }
  return std::make_unique<Tree>(Tree{"Factor", std::move(children)});
}
