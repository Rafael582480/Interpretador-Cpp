#pragma once
#include <string>
#include <vector>
#include <iostream>

class Lexer
{

public:
  enum class TokenType
  {
    String,
    Number,
    Identifier,

    Plus,
    Minus,
    Multiply,
    Divide,

    Aspas,
    LeftParen,
    RightParen,
    Semicolon,

    Print,

    EndOfFile
  };

  struct Tokens
  {
    TokenType type;
    std::string lexeme;
  };

  struct TOKENS
  {
    std::vector<Tokens> EXPRESSION;
  };

  TOKENS tokens;

  Lexer(std::string line)
  {
    TransformTokens(line);
  }

  TOKENS getTokens() const
  {
    return tokens;
  }

  void TransformTokens(std::string line);
};
