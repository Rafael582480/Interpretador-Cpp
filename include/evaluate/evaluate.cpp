#include "evaluate.hpp"
#include "../parser/parser.hpp"
#include "../environment/environment.hpp"
#include <memory>
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
    else if (identifier.node->type == "number")
    {
      int val = PrintEvaluate(*identifier.node, 0);

      std::cout << val << std::endl;
    }
    else if (identifier.node->type == "var")
    {
      std::variant<int, std::string, bool> val = environment.GetVAR(identifier.node->value);
      std::visit([](const auto &value){ std::cout << value << std::endl; }, val);
    }
  }
  else if (identifier.type == "var")
  {
    if (identifier.node->type != "String")
    {
      int val = PrintEvaluate(*identifier.node, 0);
      environment.CreatingVAR(identifier.identifer, val);
    }
    else
    {
      environment.CreatingVAR(identifier.identifer, identifier.node->value);
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

  return 0;
}
