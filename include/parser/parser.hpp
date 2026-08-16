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

    std::string identifer;

    std::unique_ptr<Node> node;
  };

  std::unique_ptr<Node> ParserPrint(Lexer::TOKENS Tokens);

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
    else if (identifier == "var")
    {
      identifierNode.type = "var";
      identifierNode.identifer = tokens.EXPRESSION[0].lexeme;

      std::vector<Lexer::Tokens> valueTokens;

      Lexer::TOKENS valueTokStruct;

      bool pass = false;

      for (size_t i = 0; i < tokens.EXPRESSION.size(); i++)
      {
        if (tokens.EXPRESSION[i].lexeme == "=")
        {
          pass = true;
          continue;
        }

        if (pass)
        {
          valueTokStruct.EXPRESSION.push_back(tokens.EXPRESSION[i]);
        }
      }

      identifierNode.node = ParserPrint(valueTokStruct);
    }
    else
    {
      std::cerr << "Error: Unknown identifier '" << identifier << "'." << std::endl;
    }
  }
};
