#pragma once

#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <string>
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

    std::unique_ptr<Node> node;
  };

  std::unique_ptr<Node> ParserPrint(Lexer::TOKENS tokens);

  Identifier identifierNode;
  
  ParserPrimary(Lexer::TOKENS tokens)
  {
    std::string identifier = tokens.EXPRESSION[0].lexeme;

    for (auto &s : tokens.EXPRESSION)
    {
      s.lexeme.erase(std::remove(s.lexeme.begin(), s.lexeme.end(), '('), s.lexeme.end());
      s.lexeme.erase(std::remove(s.lexeme.begin(), s.lexeme.end(), ')'), s.lexeme.end());
      s.lexeme.erase(std::remove(s.lexeme.begin(), s.lexeme.end(), ';'), s.lexeme.end());
    }
    std::vector<Lexer::Tokens> validTokens;
    for (const auto &token : tokens.EXPRESSION)
    {
      if (!token.lexeme.empty() && token.lexeme != " ")
      {
        validTokens.push_back(token);
      }
    }

    tokens.EXPRESSION = validTokens;

    tokens.EXPRESSION.erase(tokens.EXPRESSION.begin() + 0);

    if (identifier == "print")
    {
      identifierNode.type = "print";

      identifierNode.node = ParserPrint(tokens);
    }
    else
    {
      std::cerr << "Error: Unknown identifier '" << identifier << "'." << std::endl;
    }
  }

};
