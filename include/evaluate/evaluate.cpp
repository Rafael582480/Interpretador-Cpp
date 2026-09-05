#include "evaluate.hpp"
#include "../parser/parser.hpp"
#include "../environment/environment.hpp"
#include <memory>
#include <iostream>
#include <variant>
#include <type_traits>

Environment environment;

int PrintEvaluate(ParserPrimary::Node &node, int pos)
{
  if (node.type == "number")
    return std::stoi(node.value);

  if (node.type == "var")
    return std::get<int>(environment.GetVAR(node.value).value);

  if (node.type == "operator" && node.left && node.right)
  {
    const int left = PrintEvaluate(*node.left, pos + 1);
    const int right = PrintEvaluate(*node.right, pos + 1);

    if (node.value == "+")
      return left + right;
    if (node.value == "-")
      return right - left;
    if (node.value == "*")
      return right * left;
    if (node.value == "/")
      return right / left;
  }

  return 0;
}

std::variant<int, std::string, bool> ReturnValue(ParserPrimary::Node *expr)
{
  std::variant<int, std::string, bool> value;

  if (expr->type == "String")
  {
    value = expr->value;
  }
  else if (expr->type == "operator")
  {
    int val = PrintEvaluate(*expr, 0);

    value = val;
  }
  else if (expr->type == "number")
  {
    int val = PrintEvaluate(*expr, 0);

    value = val;
  }
  else if (expr->type == "var")
  {
    auto val = environment.GetVAR(expr->value);
    if (val.type == "String")
    {
      if (std::holds_alternative<std::string>(val.value))
      {
        value = std::get<std::string>(val.value);
      }
      else
      {
        value = std::get<std::string>(val.value);
      }
    }
    else if (val.type == "Equality")
    {
      std::cout << "eae" << std::endl;
      if (std::get<bool>(val.value))
      {
        value = "true";
      }
      else
      {
        value = "false";
      }
    }
    else if (val.type == "boolLiteral")
    {
      if (std::holds_alternative<std::string>(val.value) && std::get<std::string>(val.value) == "true")
      {
        value = "true";
      }
      else
      {
        value = "false";
      }
    }
    else
    {
      value = std::get<std::string>(val.value);
    }
  }
  else if (expr->left && expr->left->type == "boolLiteral")
  {
    std::cout << "eae" << std::endl;
    if (expr->left->value == "true")
    {
      value = "true";
    }
    else
    {
      value = "false";
    }
  }
  else if (expr->type == "Equality")
  {
    std::string equalityValue = expr->value;

    bool hasVar = (expr->right && expr->right->type == "var") || (expr->left && expr->left->type == "var");

    if (hasVar)
    {
      bool varIsRight = (expr->right && expr->right->type == "var");
      ParserPrimary::Node *varNode = varIsRight ? expr->right.get() : expr->left.get();
      ParserPrimary::Node *otherNode = varIsRight ? expr->left.get() : expr->right.get();

      auto val = environment.GetVAR(varNode->value);
      bool result = false;

      if (std::holds_alternative<std::string>(val.value))
      {
        std::string varValue = std::get<std::string>(val.value);
        std::string otherValue = otherNode ? otherNode->value : "";

        if (equalityValue == "==")
          result = (varValue == otherValue);
        else if (equalityValue == "!=")
          result = (varValue != otherValue);
      }
      else if (std::holds_alternative<int>(val.value))
      {
        int varValue = std::get<int>(val.value);
        int otherValue = 0;
        try
        {
          otherValue = std::stoi(otherNode ? otherNode->value : "0");
        }
        catch (...)
        {

          value = "false";
          return value;
        }

        if (equalityValue == "==")
          result = (varValue == otherValue);
        else if (equalityValue == "!=")
          result = (varValue != otherValue);
      }
      else if (std::holds_alternative<bool>(val.value))
      {
        bool varValue = std::get<bool>(val.value);
        bool otherValue = (otherNode && otherNode->value == "true");

        if (equalityValue == "==")
          result = (varValue == otherValue);
        else if (equalityValue == "!=")
          result = (varValue != otherValue);
      }

      value = result ? "true" : "false";
    }
    else
    {
      if (expr->left && expr->right)
      {
        if (equalityValue == "==")
          value = (expr->left->value == expr->right->value) ? "true" : "false";
        else if (equalityValue == "!=")
          value = (expr->left->value != expr->right->value) ? "true" : "false";
      }
    }

    return value;
  }
  return value;
}

Evaluate::Evaluate(std::vector<ParserPrimary::Node> &identifier)
{
  for (int i = 0; i < identifier.size(); i++)
  {
    if (identifier[i].type == "print")
    {
      auto &expr = identifier[i].left;
      if (!expr)
      {
        std::cout << "Error de expressao" << std::endl;
        continue;
      }

      auto value = ReturnValue(expr.get());

      std::visit([](const auto &val)
                 { std::cout << val << std::endl; },
                 value);
    }
    else if (identifier[i].type == "var")
    {
      auto &expr = identifier[i].left;
      if (!expr)
      {
        std::cout << "Error de expressao" << std::endl;
        continue;
      }

      auto value = ReturnValue(expr.get());

      environment.CreatingVAR(identifier[i].identifer, identifier[i].type, value);
    }
    else if (identifier[i].type == "If")
    {
      auto &condition = identifier[i].condition;

      bool result = false;

      result = std::get<std::string>(ReturnValue(condition.get())) == "true";

      if (result)
      {
        std::vector<ParserPrimary::Node> &teste = identifier[i].Statements;

        Evaluate eval(teste);
      }
    }
    else
    {
      std::cout << "Error de identificaçao" << std::endl;
      break;
    }
  }
}
