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
    std::string type;
  };

  struct Variaveis
  {
    std::vector<Variables> vars;
  };

  Variaveis variaveis;

  void CreatingVAR(std::string name, std::string type, std::variant<int, std::string, bool> value)
  {
    for (int i = 0; i < variaveis.vars.size(); i++)
    {
      if (variaveis.vars[i].name == name)
      {
        variaveis.vars[i].value = value;
        variaveis.vars[i].type = type;
        return;
      }
    }

    variaveis.vars.push_back({name, value, type});
  }

  Variables GetVAR(std::string name)
  {
    for (int i = 0; i < variaveis.vars.size(); i++)
    {
      if (variaveis.vars[i].name == name)
      {
        return variaveis.vars[i];
      }
    }

    std::cout << "Variável não encontrada" << std::endl;
  }
};
