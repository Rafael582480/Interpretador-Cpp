#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <variant>

#include "parser.hpp"
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"

std::unique_ptr<ParserPrimary::Node> Term(const std::vector<Lexer::Tokens> &Tokens)
{
  auto tree = std::make_unique<ParserPrimary::Node>();

  if (Tokens.empty())
    return nullptr;

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

  for (size_t i = 1; i + 1 < Tokens.size(); i++)
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
    if (Tokens.EXPRESSION[0].type == Lexer::TokenType::Number)
    {
      tree->type = "number";
    }
    else if (Tokens.EXPRESSION[0].type == Lexer::TokenType::Identifier)
    {
      tree->type = "var";
    }

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
    tree->value += Tokens.EXPRESSION[i].lexeme;
  }

  return tree;
}

std::unique_ptr<ParserPrimary::Node> Bool(Lexer::TOKENS Tokens)
{
  auto tree = std::make_unique<ParserPrimary::Node>();

  tree->type = "Equality";
  tree->value = "==";

  auto right = std::make_unique<ParserPrimary::Node>();
  if (Tokens.EXPRESSION[0].type == Lexer::TokenType::Number)
  {
    right->type = "Number";
  }
  else if (Tokens.EXPRESSION[0].type == Lexer::TokenType::Identifier)
  {
    right->type = "var";
  }
  else
  {
    right->type = "String";
  }
  right->value = Tokens.EXPRESSION[0].lexeme;

  auto left = std::make_unique<ParserPrimary::Node>();
  if (Tokens.EXPRESSION[2].type == Lexer::TokenType::Number)
  {
    left->type = "Number";
  }
  else if (Tokens.EXPRESSION[2].type == Lexer::TokenType::Identifier)
  {
    left->type = "var";
  }
  else
  {
    left->type = "String";
  }
  left->value = Tokens.EXPRESSION[2].lexeme;

  tree->right = std::move(right);
  tree->left = std::move(left);

  return tree;
}

void ParserPrimary::parserPrint(Lexer::TOKENS tokens, std::vector<Node> &Statements)
{
  int sintax = 0;

  Lexer::TOKENS expression;
  std::string TipagemPrint;

  while (current < tokens.EXPRESSION.size())
  {
    if (Check(Lexer::TokenType::LeftParen, tokens))
    {
      sintax++;
      current++;
    }
    else if (Check(Lexer::TokenType::RightParen, tokens))
    {
      sintax++;

      current++;
    }
    else if (Check(Lexer::TokenType::Semicolon, tokens))
    {
      current++;
      break;
    }
    else if (Check(Lexer::TokenType::Aspas, tokens))
    {
      TipagemPrint = "string";
      current++;
    }
    else if (Check(Lexer::TokenType::String, tokens))
    {
      TipagemPrint = "string";
      expression.EXPRESSION.push_back(tokens.EXPRESSION[current]);
      current++;
    }
    else if (Check(Lexer::TokenType::Equality, tokens))
    {
      TipagemPrint = "bool";
      expression.EXPRESSION.push_back(tokens.EXPRESSION[current]);
      current++;
    }
    else
    {
      expression.EXPRESSION.push_back(tokens.EXPRESSION[current]);
      current++;
    }
  }

  if (sintax < 2)
  {
    std::cout << "Error" << std::endl;
  }

  std::unique_ptr<ParserPrimary::Node> AST;
  if (TipagemPrint == "string")
  {
    AST = String(expression);
    Node printNode;
    printNode.type = "print";
    printNode.left = std::move(AST);
    Statements.push_back(std::move(printNode));
  }
  else if (TipagemPrint == "bool")
  {
    AST = Bool(expression);
    Node printNode;
    printNode.type = "print";
    printNode.left = std::move(AST);
    Statements.push_back(std::move(printNode));
  }
  else
  {
    AST = Expression(expression);
    Node printNode;
    printNode.type = "print";
    printNode.left = std::move(AST);
    Statements.push_back(std::move(printNode));
  }
}

void ParserPrimary::parserVar(Lexer::TOKENS tokens, std::vector<Node> &Statements)
{
  Lexer::TOKENS expression;
  std::string TipagemVariavel;
  std::string nameVariavel;

  while (current < tokens.EXPRESSION.size())
  {
    if (Check(Lexer::TokenType::Identifier, tokens))
    {
      nameVariavel = tokens.EXPRESSION[current].lexeme;
      current++;
    }
    else if (Check(Lexer::TokenType::Receives, tokens))
    {
      current++;
    }
    else if (Check(Lexer::TokenType::Semicolon, tokens))
    {
      current++;
      break;
    }
    else if (Check(Lexer::TokenType::String, tokens))
    {
      TipagemVariavel = "string";
      expression.EXPRESSION.push_back(tokens.EXPRESSION[current]);
      current++;
    }
    else if (Check(Lexer::TokenType::Equality, tokens))
    {
      TipagemVariavel = "bool";
      expression.EXPRESSION.push_back(tokens.EXPRESSION[current]);
      current++;
    }
    else
    {
      expression.EXPRESSION.push_back(tokens.EXPRESSION[current]);
      current++;
    }
  }

  std::unique_ptr<ParserPrimary::Node> AST;

  if (TipagemVariavel == "string")
  {
    AST = String(expression);

    Node varNode;
    varNode.identifer = nameVariavel;
    varNode.type = "var";
    varNode.left = std::move(AST);
    Statements.push_back(std::move(varNode));
  }
  else if (TipagemVariavel == "bool")
  {
    AST = Bool(expression);

    Node varNode;
    varNode.identifer = nameVariavel;
    varNode.type = "var";
    varNode.left = std::move(AST);
    Statements.push_back(std::move(varNode));
  }
  else
  {
    AST = Expression(expression);

    Node varNode;
    varNode.identifer = nameVariavel;
    varNode.type = "var";
    varNode.left = std::move(AST);
    Statements.push_back(std::move(varNode));
  }
}

void ParserPrimary::parserIf(Lexer::TOKENS tokens, std::vector<Node> &Statements)
{
  Lexer::TOKENS condition;
  bool hasCondition = false;
  std::vector<Lexer::Tokens> body;

  ParserPrimary::Node ifStatement;

  while (current < tokens.EXPRESSION.size())
  {
    if (!hasCondition)
    {
      if (Check(Lexer::TokenType::LeftParen, tokens))
      {
        current++;
      }
      else if (Check(Lexer::TokenType::RightParen, tokens))
      {
        current++;
        hasCondition = true;
      }
      else
      {
        condition.EXPRESSION.push_back(tokens.EXPRESSION[current]);
        current++;
      }
    }
    else
    {
      if (Check(Lexer::TokenType::OpenBlock, tokens))
      {
        current++;
      }
      else if (Check(Lexer::TokenType::EndBlock, tokens))
      {
        current++;
        break;
      }
      else
      {
        body.push_back(tokens.EXPRESSION[current]);
        current++;
      }
    }
  }

  parserStatement(Lexer::TOKENS{body}, ifStatement.Statements);
  ifStatement.condition = condition.EXPRESSION;

  Statements.push_back(std::move(ifStatement));
}

bool ParserPrimary::Check(Lexer::TokenType type, Lexer::TOKENS tokens)
{
  if (current >= tokens.EXPRESSION.size())
    return false;
  return tokens.EXPRESSION[current].type == type;
}

void ParserPrimary::parserStatement(Lexer::TOKENS tokens, std::vector<Node> &Statements)
{
  if (Check(Lexer::TokenType::Print, tokens))
  {
    current++;
    parserPrint(tokens, Statements);
  }
  else if (Check(Lexer::TokenType::Var, tokens))
  {
    current++;
    parserVar(tokens, Statements);
  }
  else if (Check(Lexer::TokenType::If, tokens))
  {
    std::cout << "If statement not implemented yet" << std::endl;
    current++;
    parserIf(tokens, Statements);
  }
  else
  {
    current++;
  }
};
