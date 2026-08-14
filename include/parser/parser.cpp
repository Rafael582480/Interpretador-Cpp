#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "parser.hpp"
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"

std::unique_ptr<ParserPrimary::Node> Term(std::vector<std::string> Tokens)
{
  auto tree = std::make_unique<ParserPrimary::Node>();
  tree->type = "number";
  tree->value = Tokens[0];

  if (Tokens.size() == 1)
  {
    return tree;
  }

  for (int i = 1; i < Tokens.size(); i++)
  {
    auto newTree = std::make_unique<ParserPrimary::Node>();
    if (Tokens[i] == "*" || Tokens[i] == "/")
    {
      newTree->type = "operator";
      newTree->value = Tokens[i];

      auto right = std::make_unique<ParserPrimary::Node>();
      right->type = "number";
      right->value = Tokens[i + 1];
      newTree->right = std::move(right);

      newTree->left = std::move(tree);

      tree = std::move(newTree);
    }
  };

  return tree;
};

std::unique_ptr<ParserPrimary::Node> ParserPrimary::ParserPrint(Lexer::TOKENS Tokens)
{
  auto tree = std::make_unique<Node>();
  std::vector<std::string> tks;

  for (int i = 0; i < Tokens.EXPRESSION.size(); i++)
  {
    auto newTree = std::make_unique<Node>();
    if (Tokens.EXPRESSION[i].lexeme != "+" && Tokens.EXPRESSION[i].lexeme != "-")
    {
      tks.push_back(Tokens.EXPRESSION[i].lexeme);
      continue;
    }

    newTree->type = "operator";
    newTree->value = Tokens.EXPRESSION[i].lexeme;

    newTree->right = Term(tks);
    newTree->left = std::move(tree);

    tree = std::move(newTree);
    tks.clear();
  }

  tree->left = Term(tks);

  return tree;
}
