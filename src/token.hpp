/*
A token class for parsing.
*/

#pragma once

#include <cstdint>
#include <string>

/*
A single item yielded by a lexer. This contains some information
about its origin, namely the file, column and line positions,
and text.
*/
class Token
{
  public:
    Token(const std::string &_str = "",
          const std::string &_file = "",
          const uint64_t &_line = 0, const uint64_t &_col = 0)
        : str(_str), file(_file), line(_line), col(_col)
    {
    }

    std::string str, file;
    uint64_t line, col;
};
