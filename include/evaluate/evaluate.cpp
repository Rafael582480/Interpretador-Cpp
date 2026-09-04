#include "evaluate.hpp"
#include "../parser/parser.hpp"
#include "../environment/environment.hpp"
#include <memory>
#include <iostream>
#include <variant>
#include <type_traits>

Environment environment;

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

      if (expr->type == "String")
      {
        std::cout << expr->value << std::endl;
      }
      else if (expr->type == "operator")
      {
        int val = PrintEvaluate(*expr, 0);

        std::cout << val << std::endl;
      }
      else if (expr->type == "number")
      {
        int val = PrintEvaluate(*expr, 0);

        std::cout << val << std::endl;
      }
      else if (expr->type == "var")
      {
        auto val = environment.GetVAR(expr->value);
        if (val.type == "String")
        {
          std::visit([](const auto &value)
                     { std::cout << value << std::endl; }, val.value);
        }
        else if (val.type == "Equality")
        {
          if (std::get<bool>(val.value))
          {
            std::cout << "true" << std::endl;
          }
          else
          {
            std::cout << "false" << std::endl;
          }
        }
        else if (val.type == "boolLiteral")
        {
          if (std::holds_alternative<std::string>(val.value) && std::get<std::string>(val.value) == "true")
          {
            std::cout << "true" << std::endl;
          }
          else
          {
            std::cout << "false" << std::endl;
          }
        }
        else
        {
          std::visit([](const auto &value)
                     { std::cout << value << std::endl; }, val.value);
        }
      }
      else if (expr->left->type == "boolLiteral")
      {
        std::cout << "eae" << std::endl;
        if (expr->left->value == "true")
        {
          std::cout << "true" << std::endl;
        }
        else
        {
          std::cout << "false" << std::endl;
        }
      }
      else if (expr->type == "Equality")
      {
        if (expr->right && (expr->right->type == "var" || (expr->left && expr->left->type == "var")))
        {
          if (expr->right->type == "var")
          {
            auto val = environment.GetVAR(expr->right->value);
            if (std::holds_alternative<std::string>(val.value))
            {
              if (expr->left && std::get<std::string>(val.value) == expr->left->value)
                std::cout << "true" << std::endl;
              else
                std::cout << "false" << std::endl;
            }
            else if (std::holds_alternative<int>(val.value))
            {
              if (expr->left && std::to_string(std::get<int>(val.value)) == expr->left->value)
                std::cout << "true" << std::endl;
              else
                std::cout << "false" << std::endl;
            }
            else if (std::holds_alternative<bool>(val.value))
            {
              if (expr->left && (std::get<bool>(val.value) ? "true" : "false") == expr->left->value)
                std::cout << "true" << std::endl;
              else
                std::cout << "false" << std::endl;
            }
            else
            {
              std::cout << "false" << std::endl;
            }
          }
          else if (expr->left)
          {
            auto val = environment.GetVAR(expr->left->value);
            if (std::holds_alternative<std::string>(val.value))
            {
              if (expr->right && std::get<std::string>(val.value) == expr->right->value)
                std::cout << "true" << std::endl;
              else
                std::cout << "false" << std::endl;
            }
            else if (std::holds_alternative<int>(val.value))
            {
              if (expr->right && std::to_string(std::get<int>(val.value)) == expr->right->value)
                std::cout << "true" << std::endl;
              else
                std::cout << "false" << std::endl;
            }
            else if (std::holds_alternative<bool>(val.value))
            {
              if (expr->right && (std::get<bool>(val.value) ? "true" : "false") == expr->right->value)
                std::cout << "true" << std::endl;
              else
                std::cout << "false" << std::endl;
            }
            else
            {
              std::cout << "false" << std::endl;
            }
          }
        }
        else if (expr->right && expr->left)
        {
          if (expr->right->value == expr->left->value)
          {
            std::cout << "true" << std::endl;
          }
          else
          {
            std::cout << "false" << std::endl;
          }
        }
      }
    }
    else if (identifier[i].type == "var")
    {
      if (identifier[i].left->type == "Equality")
      {
        bool result = identifier[i].left->left->value == identifier[i].left->right->value;

        environment.CreatingVAR(identifier[i].identifer, "Equality", result);
      }
      else if (identifier[i].left->type == "boolLiteral")
      {
        environment.CreatingVAR(identifier[i].identifer, "boolLiteral", identifier[i].left->value);
      }
      else if (identifier[i].left->type != "String")
      {
        int val = PrintEvaluate(*identifier[i].left, 0);

        environment.CreatingVAR(
            identifier[i].identifer,
            identifier[i].type,
            val);
      }
      else
      {
        environment.CreatingVAR(
            identifier[i].identifer,
            identifier[i].type,
            identifier[i].left->value);
      }
    }
    else if (identifier[i].type == "If")
    {
      auto &condition = identifier[i].condition;

      bool result = false;
      if (condition->type == "Equality")
      {
        if (condition->left->type == "var" && condition->right->type == "var")
        {
          auto leftValue = environment.GetVAR(condition->left->value).value;

          auto rightValue = environment.GetVAR(condition->right->value).value;

          result = leftValue == rightValue;
        }
        else if (condition->left->type == "var")
        {
          auto leftValue =
              environment.GetVAR(condition->left->value).value;

          result = std::visit(
              [&](const auto &value)
              {
                using T = std::decay_t<decltype(value)>;

                if constexpr (std::is_same_v<T, int>)
                {
                  return std::stoi(condition->right->value) == value;
                }
                else if constexpr (std::is_same_v<T, std::string>)
                {
                  return condition->right->value == value;
                }
                else if constexpr (std::is_same_v<T, bool>)
                {
                  return condition->right->value ==
                         (value ? "true" : "false");
                }
              },
              leftValue);
        }
        else if (condition->right->type == "var")
        {
          auto rightValue =
              environment.GetVAR(condition->right->value).value;

          result = std::visit(
              [&](const auto &value)
              {
                using T = std::decay_t<decltype(value)>;

                if constexpr (std::is_same_v<T, int>)
                {
                  return condition->left->value == std::to_string(value);
                }
                else if constexpr (std::is_same_v<T, std::string>)
                {
                  return condition->left->value == value;
                }
                else if constexpr (std::is_same_v<T, bool>)
                {
                  return condition->left->value ==
                         (value ? "true" : "false");
                }
              },
              rightValue);
        }

        else
        {
          result =
              condition->left->value ==
              condition->right->value;
        }
      }
      else if (condition->type == "var")
      {
        auto val = environment.GetVAR(condition->value);

        if (val.type == "Equality")
        {
          if (std::get<bool>(val.value))
          {
            result = true;
          }
          else
          {
            result = false;
          }
        }
        else if (val.type == "boolLiteral")
        {
          if (std::holds_alternative<std::string>(val.value) && std::get<std::string>(val.value) == "true")
          {
            result = true;
          }
          else
          {
            result = false;
          }
        }
        else
        {
          std::visit([](const auto &value)
                     { std::cout << value << std::endl; }, val.value);
        }
      }
      else
      {
        auto val = environment.GetVAR(condition->value);

        result = std::get<bool>(val.value);
      }

      if (result)
      {
        std::vector<ParserPrimary::Node> &teste = identifier[i].Statements;

        Evaluate eval(teste);
      }
    }
    else
    {
      std::cout << "Error de identificaçao" << std::endl;
    }
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
