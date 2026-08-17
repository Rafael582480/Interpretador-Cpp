#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "parser.hpp"
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"

std::unique_ptr<ParserPrimary::Node> Term(const std::vector<Lexer::Tokens> &Tokens)
{
  auto tree = std::make_unique<ParserPrimary::Node>();
  if (Tokens[0].type == Lexer::TokenType::Number)
  {
    tree->type = "number";
    tree->value = Tokens[0].lexeme;
  }
  else if (Tokens[0].type == Lexer::TokenType::Identifier)
  {
    tree->type = "var";
    tree->value = Tokens[0].lexeme;
  }

  if (Tokens.size() == 1)
  {
    return tree;
  }

  for (int i = 1; i < Tokens.size(); i++)
  {

    auto newTree = std::make_unique<ParserPrimary::Node>();

    if (Tokens[i].lexeme == "*" || Tokens[i].lexeme == "/")
    {
      newTree->type = "operator";
      newTree->value = Tokens[i].lexeme;

      auto right = std::make_unique<ParserPrimary::Node>();

      if (Tokens[i + 1].type == Lexer::TokenType::Number)
      {
        right->type = "number";
      }
      else if (Tokens[i + 1].type == Lexer::TokenType::Identifier)
      {
        right->type = "var";
      }

      right->value = Tokens[i + 1].lexeme;

      newTree->right = std::move(right);

      newTree->left = std::move(tree);

      tree = std::move(newTree);
    }
  };

  return tree;
};

std::unique_ptr<ParserPrimary::Node> Expression(Lexer::TOKENS Tokens)
{
  auto tree = std::make_unique<ParserPrimary::Node>();

  if (Tokens.EXPRESSION.size() == 1)
  {

    tree->type = "number";
    tree->value = Tokens.EXPRESSION[0].lexeme;

    return tree;
  }

  std::vector<Lexer::Tokens> tks;

  for (int i = 0; i < Tokens.EXPRESSION.size(); i++)
  {
    auto newTree = std::make_unique<ParserPrimary::Node>();
    if (Tokens.EXPRESSION[i].lexeme != "+" && Tokens.EXPRESSION[i].lexeme != "-")
    {
      tks.push_back(Tokens.EXPRESSION[i]);
      continue;
    }

    newTree->type = "operator";
    newTree->value = Tokens.EXPRESSION[i].lexeme;

    newTree->right = Term(tks);
    newTree->left = std::move(tree);

    tree = std::move(newTree);
    tks.clear();
  }

  if (tree->value.empty())
  {
    return Term(tks);
  }

  tree->left = Term(tks);

  return tree;
}

std::unique_ptr<ParserPrimary::Node> String(Lexer::TOKENS Tokens)
{
  auto tree = std::make_unique<ParserPrimary::Node>();

  tree->type = "String";

  for (int i = 0; i < Tokens.EXPRESSION.size(); i++)
  {
    if (Tokens.EXPRESSION[i].lexeme == "'")
    {
      continue;
    }

    tree->value += Tokens.EXPRESSION[i].lexeme;
  }

  return tree;
}

std::unique_ptr<ParserPrimary::Node> ParserPrimary::ParserPrint(Lexer::TOKENS Tokens)
{
  auto tree = std::make_unique<ParserPrimary::Node>();

  if (Tokens.EXPRESSION[0].type == Lexer::TokenType::String)
  {
    tree = String(Tokens);
  }
  else if (Tokens.EXPRESSION[0].type == Lexer::TokenType::Number)
  {
    tree = Expression(Tokens);
  }
  else
  {
    if (Tokens.EXPRESSION.size() == 1)
    {
      tree->type = "var";
      tree->value = Tokens.EXPRESSION[0].lexeme;
    }
    else
    {
      tree = Expression(Tokens);
    }
  }

  return tree;
}
