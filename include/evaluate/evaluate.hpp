#pragma once

#include <iostream>
#include "../parser/parser.hpp"

class Evaluate
{
public:
  Evaluate(ParserPrimary::Identifier identifier);

  int PrintEvaluate(ParserPrimary::Node& node, int pos);
};
