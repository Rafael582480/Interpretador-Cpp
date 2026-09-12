#pragma once
#include <iostream>
#include <string>
#include <stdexcept>
#include <variant>
#include <vector>

class Environment
{
private:
  Environment *parent;

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

  Environment(Environment *parent = nullptr) : parent(parent)
  {
  }

  void CreatingVAR(std::string name, std::string type, std::variant<int, std::string, bool> value)
  {
    for (std::size_t i = 0; i < variaveis.vars.size(); i++)
    {
      if (variaveis.vars[i].name == name)
      {
        throw std::runtime_error("Variável já existe!");
        return;
      }
    }

    variaveis.vars.push_back({name, value, type});
  }

  void SetVAR(std::string name, std::string type, std::variant<int, std::string, bool> value)
  {
    for (std::size_t i = 0; i < variaveis.vars.size(); i++)
    {
      if (variaveis.vars[i].name == name)
      {
        variaveis.vars[i].value = value;
        variaveis.vars[i].type = type;
        return;
      }
    }

    if (parent != nullptr)
    {
      parent->SetVAR(name, type, value);
      return;
    }

    throw std::runtime_error("Variável não existe!");
  }

  Variables GetVAR(std::string name)
  {
    for (std::size_t i = 0; i < variaveis.vars.size(); i++)
    {
      if (variaveis.vars[i].name == name)
      {
        return variaveis.vars[i];
      }
    }

    if (parent != nullptr)
    {
      return parent->GetVAR(name);
    }

    throw std::runtime_error("Variável não encontrada");
  }
};
