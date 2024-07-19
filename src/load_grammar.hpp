/*
Function for loading grammar files into grammar objects.
*/

#pragma once

#include "grammar.hpp"
#include "lexer.hpp"
#include <string>

void load_grammar(const std::string &_path, Lexer &_l,
                  Grammar &_g);
