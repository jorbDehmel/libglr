/*
I think this is technically recursive descent?

Outlines a parser class. This will contain an object
representing a compiled grammar as a graph. A series of cursors
will move through this graph. A given node will be considered
canonical iff all valid cursors flow through it. Each node will
represent a single regular expression terminal, and the edges
exiting it represent transitions determined by the grammar. Each
cursor will keep track of both its node and token histories. If
there are multiple choices at a given location, the cursor will
split. There should be exactly one canonical flow through the
graph at the end of a parse.

Jordan Dehmel, 2024
jdehmel@outlook.com
MIT License
This software was created as a creative exercise.
*/

#pragma once

#include "cursor.hpp"
#include "grammar.hpp"
#include "token.hpp"
#include <set>

class Parser
{
  public:
    Parser(const Grammar &_grammar);

    void reset();
    void process_token(const Token &_what);

    int n_viable() const noexcept;
    Cursor finalize();

    void graphviz(const std::string &_filepath) const noexcept;

  protected:
    const GrammarGraph g;
    std::set<Cursor> cursors;
};
