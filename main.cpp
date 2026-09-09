#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cctype>

#include "./include/lexer/lexer.hpp"
#include "./include/parser/parser.hpp"
#include "./include/evaluate/evaluate.hpp"

int main()
{
  using namespace std::chrono;

  auto inicioTotal = high_resolution_clock::now();

  // =========================
  // Leitura do arquivo
  // =========================

  auto inicioArquivo = high_resolution_clock::now();

  std::string source;
  std::string line;
  std::ifstream Arquivo("./test/txt.txt");

  while (getline(Arquivo, line))
  {
    if (line.empty() ||
        std::all_of(line.begin(), line.end(),
                    [](unsigned char c)
                    {
                      return std::isspace(c);
                    }))
    {
      continue;
    }

    source += line;
    source += '\n';
  }

  auto fimArquivo = high_resolution_clock::now();

  // =========================
  // Lexer
  // =========================

  auto inicioLexer = high_resolution_clock::now();

  Lexer lexer(source);
  Lexer::TOKENS tokens = lexer.getTokens();

  auto fimLexer = high_resolution_clock::now();

  // =========================
  // Parser
  // =========================

  auto inicioParser = high_resolution_clock::now();

  ParserPrimary parser(tokens);

  auto fimParser = high_resolution_clock::now();

  // =========================
  // Evaluate
  // =========================

  auto inicioEvaluate = high_resolution_clock::now();
  Environment environment;
  Evaluate evaluate(parser.Statements, environment);

  auto fimEvaluate = high_resolution_clock::now();

  // =========================
  // Resultados
  // =========================

  auto fimTotal = high_resolution_clock::now();

  auto tempoArquivo =
      duration_cast<microseconds>(fimArquivo - inicioArquivo);

  auto tempoLexer =
      duration_cast<microseconds>(fimLexer - inicioLexer);

  auto tempoParser =
      duration_cast<microseconds>(fimParser - inicioParser);

  auto tempoEvaluate =
      duration_cast<microseconds>(fimEvaluate - inicioEvaluate);

  auto tempoTotal =
      duration_cast<microseconds>(fimTotal - inicioTotal);

  std::cout << "\n========== PERFORMANCE ==========\n";

  std::cout << "Arquivo:   "
            << tempoArquivo.count()
            << " us\n";

  std::cout << "Lexer:     "
            << tempoLexer.count()
            << " us\n";

  std::cout << "Parser:    "
            << tempoParser.count()
            << " us\n";

  std::cout << "Evaluate:  "
            << tempoEvaluate.count()
            << " us\n";

  std::cout << "Total:     "
            << tempoTotal.count()
            << " us\n";

  std::cout << "================================\n";

  Arquivo.close();

  return 0;
}
