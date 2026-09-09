#pragma once

#include <iostream>
#include "../parser/parser.hpp"
#include "../environment/environment.hpp"

class Evaluate
{
public:

  Evaluate(std::vector<ParserPrimary::Node>& identifier, Environment &environmentScopo);

};
