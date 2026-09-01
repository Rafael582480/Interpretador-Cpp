#pragma once

#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

#include "../lexer/lexer.hpp"

class ParserPrimary
{
public:
  struct Node
  {
    std::vector<Lexer::Tokens> condition;
    std::vector<Node> Statements;
    std::string identifer;
    std::string type;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    std::string value;
  };

  std::vector<Node> Statements;
  int current = 0;

  ParserPrimary(Lexer::TOKENS tokens)
  {
    while (current < tokens.EXPRESSION.size())
    {
      parserStatement(tokens, Statements);
    }
  }

  void parserStatement(Lexer::TOKENS tokens, std::vector<Node> &Statements);

  void parserPrint(Lexer::TOKENS, std::vector<Node> &Statements);
  void parserVar(Lexer::TOKENS, std::vector<Node> &Statements);
  void parserIf(Lexer::TOKENS, std::vector<Node> &Statements);

  bool Check(Lexer::TokenType type, Lexer::TOKENS tokens);
};
