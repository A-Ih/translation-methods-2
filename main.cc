#include "parser.hh"

#include <fstream>
#include <iostream>
#include <memory>
#include <cassert>

int main(int argc, char* argv[]) {
  std::shared_ptr<std::istream> source;
  if (argc >= 2) {
    // We have received the name of the file to parse as a first argument
    source = std::make_shared<std::ifstream>(std::string{argv[1]});
  } else {
    // We should read from the input
    source = std::shared_ptr<std::istream>(&std::cin, [](auto) {});
    // custom noop deleter for std::cin
  }
  try {
    *source >> std::noskipws;
    Parser parser(source);
    auto result = parser.Parse();
    assert(result);
    TreeToDot(std::cout, result.get());
  } catch (const ParserException& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}
