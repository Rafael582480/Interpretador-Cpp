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

    Receives,

    Aspas,
    LeftParen,
    RightParen,
    Semicolon,
    Equality,

    Name,
    Print,
    Var,
    If,
    Open,
    End,

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
