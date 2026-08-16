#pragma once
#include <iostream>
#include <string>
#include <variant>
#include <vector>

class Environment
{
public:
  struct Variables
  {
    std::string name;
    std::variant<int, std::string, bool> value;
  };

  struct Variaveis
  {
    std::vector<Variables> vars;
  };

  Variaveis variaveis;

  void CreatingVAR(std::string name, std::variant<int, std::string, bool> value)
  {
    variaveis.vars.push_back({ name, value });
  }

  std::variant<int, std::string, bool> GetVAR(std::string name)
  {
    for (int i = 0; i < variaveis.vars.size(); i++)
    {
      if (variaveis.vars[i].name == name)
      {
        return variaveis.vars[i].value;
      }
    }

    return "Variable not declaration";
  }
};
