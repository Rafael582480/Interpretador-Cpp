#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include "./include/lexer/lexer.hpp"
#include "./include/parser/parser.hpp"
#include "./include/evaluate/evaluate.hpp"

int main()
{
  std::string line;
  std::ifstream Arquivo("./test/txt.txt");

  while (getline(Arquivo, line))
  {
    if (line.empty() || std::all_of(line.begin(), line.end(), [](unsigned char c) { return std::isspace(c); }))
    {
      continue;
    }

    Lexer lexer(line);
    Lexer::TOKENS token = lexer.getTokens();

    if (token.EXPRESSION.empty())
    {
      continue;
    }

    ParserPrimary parser(token);

    Evaluate evaluate(std::move(parser.identifierNode));
  }

  Arquivo.close();

  return 0;
}
