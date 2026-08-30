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
    std::string type;

    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    std::string value;
  };

  struct Identifier
  {
    std::string type;

    std::string identifer;

    std::unique_ptr<Node> node;
  };

  Identifier identifierNode;

  std::vector<Identifier> Statements;
  int current = 0;

  ParserPrimary(Lexer::TOKENS tokens)
  {
    while(current < tokens.EXPRESSION.size())
    {
      parserStatement(tokens);
    }
  }

  void parserStatement(Lexer::TOKENS tokens);

  void parserPrint(Lexer::TOKENS);
  void parserVar(std::vector<Lexer::Tokens> tokens);

  bool Check(Lexer::TokenType type, Lexer::TOKENS tokens);
};
