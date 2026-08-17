#include "evaluate.hpp"
#include "../parser/parser.hpp"
#include "../environment/environment.hpp"
#include <memory>
#include <iostream>
#include <variant>

Environment environment;

Evaluate::Evaluate(ParserPrimary::Identifier identifier)
{
  if (identifier.type == "print")
  {
    if (identifier.node->type == "String")
    {
      std::cout << identifier.node->value << std::endl;
    }
    else if (identifier.node->type == "operator")
    {
      int val = PrintEvaluate(*identifier.node, 0);

      std::cout << val << std::endl;
    }
    else if (identifier.node->type == "number")
    {
      int val = PrintEvaluate(*identifier.node, 0);

      std::cout << val << std::endl;
    }
    else if (identifier.node->type == "var")
    {
      auto val = environment.GetVAR(identifier.node->value);

      if (val.type == "String")
      {
        std::visit([](const auto &value)
                   { std::cout << value << std::endl; }, val.value);
      }
      else
      {
        std::visit([](const auto &value)
                   { std::cout << value << std::endl; }, val.value);
      }
    }
  }
  else if (identifier.type == "var")
  {
    if (identifier.node->type != "String")
    {
      int val = PrintEvaluate(*identifier.node, 0);
      environment.CreatingVAR(identifier.identifer, identifier.node->type, val);
    }
    else
    {
      environment.CreatingVAR(identifier.identifer, identifier.node->type, identifier.node->value);
    }
  }
  else
  {
    std::cout << "Error de identificaçao" << std::endl;
  }
}

int printNodeInfo(ParserPrimary::Node &node, int pos)
{
  return std::stoi(node.value);
}

int Evaluate::PrintEvaluate(ParserPrimary::Node &node, int pos = 0)
{
  if (node.type == "String")
  {
    std::cout << node.value << std::endl;
  }

  if (node.type == "number")
  {
    return printNodeInfo(node, pos);
  }

  if (node.type == "operator")
  {
    int left = PrintEvaluate(*node.left, pos + 1);
    int right = PrintEvaluate(*node.right, pos + 1);

    if (node.value == "+")
    {
      return right + left;
    }
    else if (node.value == "-")
    {
      return right - left;
    }
    else if (node.value == "*")
    {
      return right * left;
    }
    else if (node.value == "/")
    {
      return right / left;
    }
  }

  if (node.type == "var")
  {
    auto val = environment.GetVAR(node.value);

    return std::get<int>(val.value);
  }

  return 0;
}
