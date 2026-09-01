#pragma once

#include <iostream>
#include "../parser/parser.hpp"

class Evaluate
{
public:
  Evaluate(std::vector<ParserPrimary::Node>& identifier);

  int PrintEvaluate(ParserPrimary::Node& node, int pos);
};
