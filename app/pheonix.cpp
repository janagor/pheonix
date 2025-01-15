#include "ast_view.hpp"
#include "evaluator.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "repl.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<pheonix::lexer::Lexem> lexerize(pheonix::lexer::Lexer &lexer) {
  std::vector<pheonix::lexer::Lexem> result;
  while (true) {
    pheonix::lexer::Lexem l = lexer.nextLexem();
    result.emplace_back(l);
    if (l.token.getTokenType() == pheonix::token::TokenType::END_OF_FILE) {
      return result;
    }
  }
}

void help() {
  std::cout << "Usage: ./pheonix [OPTIONS]" << std::endl << std::endl;
  std::cout << "    -h           Display this message" << std::endl;
  std::cout << "    -p INPUT     Generate parser output" << std::endl;
  std::cout << "    -l INPUT     Generate lexer output" << std::endl;
  std::cout << "    -i INPUT     Generate interpreter output" << std::endl;
}

int main(int argc, char **argv) {
  if (argc <= 1) {
    pheonix::repl::repl();
  }

  if (argc == 2 && std::string(argv[1]) == "-h") {
    help();
    return 0;
  }

  if (argc == 3 && std::string(argv[1]) == "-l") {
    std::string filename = std::string(argv[2]);
    std::ifstream input(filename);
    if (!input) {
      std::cerr << "Error: Could not open file.\n";
      return 1; // Exit with error
    }

    pheonix::lexer::Lexer l(input);
    std::vector<pheonix::lexer::Lexem> result = lexerize(l);
    for (const auto &lexem : result) {
      std::cout << lexem << std::endl;
    }
    std::cout << std::endl;
    input.close();
    return 0;
  }

  if (argc == 3 && std::string(argv[1]) == "-p") {
    std::string filename = std::string(argv[2]);
    std::ifstream input(filename);
    if (!input) {
      std::cerr << "Error: Could not open file.\n";
      return 1;
    }
    pheonix::parser::Parser p(input);

    std::unique_ptr<pheonix::node::Node> output = p.generateParsingTree();
    pheonix::ast_view::ASTView visitor;
    output->accept(visitor);
    std::string received = visitor.getResult();

    std::cout << received << std::endl;
    input.close();
    return 0;
  }

  if (argc == 3 && std::string(argv[1]) == "-i") {
    std::string filename = std::string(argv[2]);
    std::ifstream input(filename);
    if (!input) {
      std::cerr << "Error: Could not open file.\n";
      return 1;
    }
    pheonix::parser::Parser p(input);

    std::unique_ptr<pheonix::node::Node> output = p.generateParsingTree();
    pheonix::eval::Evaluator evaluator;
    output->accept(evaluator);
    input.close();
    return 0;
  }

  std::cout << "Wrong params." << std::endl;
  help();
  return 1;
}
