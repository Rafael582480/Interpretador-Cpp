#include "evaluate.hpp"
#include "../parser/parser.hpp"
#include <memory>

Evaluate::Evaluate(ParserPrimary::Identifier identifier)
{
  if (identifier.type == "print")
  {
    int result = 0;
    int val = PrintEvaluate(*identifier.node, 0);

    std::cout << val << std::endl;
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
      return left + right;
    }
  }
  
  return 0;
}
