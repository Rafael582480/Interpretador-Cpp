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
    std::unique_ptr<Node> condition;
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
      parserStatement(tokens, Statements, current);
    }
  }

  void parserStatement(Lexer::TOKENS tokens, std::vector<Node> &Statements, int &current);

  void parserPrint(Lexer::TOKENS, std::vector<Node> &Statements, int &current);
  void parserVar(Lexer::TOKENS, std::vector<Node> &Statements, int &current);
  void parserIf(Lexer::TOKENS, std::vector<Node> &Statements, int &current);

  bool Check(Lexer::TokenType type, Lexer::TOKENS tokens, int &current);
};
