/*
A cursor class for the traversal of compiled parse graphs. This
keeps track of its node and token histories and has a current
node it occupies.
*/

#pragma once

#include "node.hpp"
#include "token.hpp"
#include <list>
#include <set>
#include <stack>

// The max number of calls before a cursor throws an error.
const static uint64_t max_cursor_recursion_depth = 512;

/*
An object which keeps track of a history and call stack within
a parse graph.
*/
class Cursor
{
  public:
    // Entry node constructor
    Cursor(ParseNode *const _cur);

    // Non-entry node constructor
    Cursor(const Cursor &_parent, ParseNode *const _cur,
           const Token &_last_tok,
           const std::stack<ParseNode *> &_call_stack);

    // For stl
    operator size_t() const;

    // Returns a set of all possible next nodes. Each of these
    // should be allocated a new cursor.
    std::set<Cursor> next_cursors(const Token &_next_tok) const;

    // Returns whether or not this history represents a valid
    // parse.
    bool is_valid_end() const noexcept;

    // Each item in this list is a node paired with the token
    // which caused this cursor to move **to** it. The first
    // item in the list is the entry node, and the Token
    // associated with it is meaningless. Assume it is the unit
    // string.
    const std::list<std::pair<ParseNode *const, Token>> history;
    const std::stack<ParseNode *> call_stack;
};
