#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "./include/lexer/lexer.hpp"
#include "./include/parser/parser.hpp"
#include "./include/evaluate/evaluate.hpp"

int main()
{
  std::string source;
  std::string line;
  std::ifstream Arquivo("./test/txt.txt");

  while (getline(Arquivo, line))
  {
    if (line.empty() || std::all_of(line.begin(), line.end(), [](unsigned char c) { return std::isspace(c); }))
    {
      continue;
    }
    source+=line;
    source+='\n';
  }

  Lexer lexer(source);
  Lexer::TOKENS tokens = lexer.getTokens();

  ParserPrimary parser(tokens);

  Evaluate evaluate(parser.Statements);

  Arquivo.close();

  return 0;
}
