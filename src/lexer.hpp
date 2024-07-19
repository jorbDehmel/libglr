/*
Defines the lexer class.
*/

#pragma once

#include "token.hpp"
#include <stack>
#include <string>

class Lexer
{
  public:
    // If true, only fetches non-whitespace tokens.
    bool ignore_whitespace = true;

    void init_file(const std::string &_path);
    bool done() const;

    Token next_token();

    void push_state_and_load(const std::string &_path);
    void pop_state();

  protected:
    struct State
    {
        std::string filename;
        uint64_t line, col;
        FILE *f = nullptr;
    };

    State cur_state;
    std::stack<State> include_stack;
};
