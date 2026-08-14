#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Parser
{
private:
  struct Node
  {
    std::string VALUE;

    std::unique_ptr<Node> LEFT;
    std::unique_ptr<Node> RIGHT;
  };

  // 1+1+1

public:
  Parser(const std::vector<std::string> Tokens)
  {
    auto tree = std::make_unique<Node>();
    std::vector<std::string> tks;

    // 1 + 1

    for (int i = 0; i < Tokens.size(); i++)
    {
      auto newTree = std::make_unique<Node>();
      if (Tokens[i] != "+")
      {
        tks.push_back(Tokens[i]);
        continue;
      }

      newTree->VALUE = Tokens[i];

      newTree->RIGHT = Term(tks);
      newTree->LEFT = std::move(tree);

      tree = std::move(newTree);
      tks.clear();
    }

    tree->LEFT = Term(tks);

    PrintAST(*tree);
  };

  std::unique_ptr<Node> Term(const std::vector<std::string> Tokens)
  {
    auto tree = std::make_unique<Node>();
    tree->VALUE = Tokens[0];

    if (Tokens.size() == 1)
    {
      tree->VALUE = Tokens[0];
      return tree;
    }

    for (int i = 1; i < Tokens.size(); i++)
    {
      auto newTree = std::make_unique<Node>();
      if (Tokens[i] == "*")
      {
        newTree->VALUE = Tokens[i];

        auto right = std::make_unique<Node>();
        right->VALUE = Tokens[i + 1];
        newTree->RIGHT = std::move(right);

        newTree->LEFT = std::move(tree);

        tree = std::move(newTree);
      }
    };

    return tree;
  };

  void PrintAST(const Node &AST)
  {
    std::cout << AST.VALUE << std::endl;

    if (AST.RIGHT)
    {
      PrintAST(*AST.RIGHT);
    }
    if (AST.LEFT)
    {
      PrintAST(*AST.LEFT);
    }
  }
};

int main()
{

  const std::vector<std::string> TOKENS = {
      "2",
      "+",
      "2",};

  Parser parser(TOKENS);

  return 0;
}
