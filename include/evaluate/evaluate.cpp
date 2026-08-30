#include "evaluate.hpp"
#include "../parser/parser.hpp"
#include "../environment/environment.hpp"
#include <memory>
#include <iostream>
#include <variant>

Environment environment;

Evaluate::Evaluate(std::vector<ParserPrimary::Identifier> identifier)
{

  for(int i=0; i < identifier.size(); i++)
  {
    if (identifier[i].type == "print")
    {
      if (identifier[i].node->type == "String")
      {
        std::cout << identifier[i].node->value << std::endl;
      }
      else if (identifier[i].node->type == "operator")
      {
        int val = PrintEvaluate(*identifier[i].node, 0);
  
        std::cout << val << std::endl;
      }
      else if (identifier[i].node->type == "number")
      {
        int val = PrintEvaluate(*identifier[i].node, 0);
  
        std::cout << val << std::endl;
      }
      else if (identifier[i].node->type == "var")
      {
        auto val = environment.GetVAR(identifier[i].node->value);
  
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
        else
        {
          std::visit([](const auto &value)
                     { std::cout << value << std::endl; }, val.value);
        }
      }
      else if (identifier[i].node->type == "Equality")
      {
        if (identifier[i].node->right->type == "var" || identifier[i].node->left->type == "var")
        {
          if (identifier[i].node->right->type == "var")
          {
            auto val = environment.GetVAR(identifier[i].node->right->value);
            if (std::holds_alternative<std::string>(val.value))
            {
              if (std::get<std::string>(val.value) == identifier[i].node->left->value)
                std::cout << "true" << std::endl;
              else
                std::cout << "false" << std::endl;
            }
            else if (std::holds_alternative<int>(val.value))
            {
              if (std::to_string(std::get<int>(val.value)) == identifier[i].node->left->value)
                std::cout << "true" << std::endl;
              else
                std::cout << "false" << std::endl;
            }
            else if (std::holds_alternative<bool>(val.value))
            {
              if ((std::get<bool>(val.value) ? "true" : "false") == identifier[i].node->left->value)
                std::cout << "true" << std::endl;
              else
                std::cout << "false" << std::endl;
            }
            else
            {
              std::cout << "false" << std::endl;
            }
          }
          else
          {
            auto val = environment.GetVAR(identifier[i].node->left->value);
            if (std::holds_alternative<std::string>(val.value))
            {
              if (std::get<std::string>(val.value) == identifier[i].node->right->value)
                std::cout << "true" << std::endl;
              else
                std::cout << "false" << std::endl;
            }
            else if (std::holds_alternative<int>(val.value))
            {
              if (std::to_string(std::get<int>(val.value)) == identifier[i].node->right->value)
                std::cout << "true" << std::endl;
              else
                std::cout << "false" << std::endl;
            }
            else if (std::holds_alternative<bool>(val.value))
            {
              if ((std::get<bool>(val.value) ? "true" : "false") == identifier[i].node->right->value)
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
        else
        {
          if (identifier[i].node->right->value == identifier[i].node->left->value)
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
      if (identifier[i].node->type == "Equality")
      {
        bool result = identifier[i].node->left->value == identifier[i].node->right->value;
  
        environment.CreatingVAR(identifier[i].identifer, "Equality", result);
      }
      else if (identifier[i].node->type != "String")
      {
        int val = PrintEvaluate(*identifier[i].node, 0);
  
        environment.CreatingVAR(
            identifier[i].identifer,
            identifier[i].node->type,
            val);
      }
      else
      {
        environment.CreatingVAR(
            identifier[i].identifer,
            identifier[i].node->type,
            identifier[i].node->value);
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
