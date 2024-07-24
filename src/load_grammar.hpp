/*
Function for loading grammar files into grammar objects.
*/

#pragma once

#include "grammar.hpp"
#include "token.hpp"
#include <list>
#include <string>

/*
Use a lexer to load a grammar from a file.
*/
Grammar load_grammar_file(const std::string &_path);

/*
Load a pre-lexed grammar. This is called by the file-based one
after lexing.
*/
Grammar load_grammar(const std::list<Token> &_text,
                     const std::string &_token_filename);
