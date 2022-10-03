#include <istream>
#include <string>

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

struct Token {
  TokenType type;
  std::string value;
};

class LexicalAnalyzer {
 public:
 private:
};
