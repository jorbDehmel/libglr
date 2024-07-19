/*
A single node in a parse graph.

Node can be:
standard node
call node
return node

If a cursor enters a call node, it pushes it to the return stack
and jumps

If a cursor enters a return node, it jumps to the top of the
return stack and pops from it. If the return stack is empty, it
validates its path as a valid parse.

If a cursor has no valid moves, it dies.
*/

#pragma once

#include <boost/regex.hpp>

enum ParseNodeType
{
    NORMAL,
    CALL,
    RETURN
};

// Does NOT take ownership of its links!
class ParseNode
{
  public:
    void set_entry_exit_points(ParseNode *const,
                               ParseNode *const) noexcept;
    void set_transitions(
        const std::list<std::pair<boost::regex, ParseNode *>>
            &) noexcept;
    void set_as_return() noexcept;

    ParseNode *const get_entry_point();
    ParseNode *const get_exit_point();
    const std::list<std::pair<boost::regex, ParseNode *>> &
    get_transitions();

    // The type of this node
    ParseNodeType type = NORMAL;

    // The data of this node
    std::list<std::pair<boost::regex, ParseNode *>> transitions;
    ParseNode *entry_point, *exit_point;
};
