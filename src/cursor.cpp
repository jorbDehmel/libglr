/*
Implementations for the cursor class.
*/

#include "cursor.hpp"
#include "node.hpp"
#include <boost/regex/v5/regex_match.hpp>
#include <stdexcept>

// Because there is not a convenient way to do this in a const
// constructor, to my knowledge
inline std::list<std::pair<ParseNode *const, Token>>
new_history(
    const std::list<std::pair<ParseNode *const, Token>>
        &_existing,
    const std::pair<ParseNode *const, Token> &_to_append)
{
    auto out = _existing;
    out.push_back(_to_append);
    return out;
}

// Entry node constructor
Cursor::Cursor(ParseNode *const _cur)
    : history(1, std::make_pair(_cur, Token()))
{
}

// Non-entry node constructor
Cursor::Cursor(const Cursor &_parent, ParseNode *const _cur,
               const Token &_last_tok,
               const std::stack<ParseNode *> &_call_stack)
    : history(new_history(_parent.history,
                          std::make_pair(_cur, _last_tok))),
      call_stack(_call_stack)
{
}

// For mapping and stuff
Cursor::operator size_t() const
{
    return (size_t)history.back().first;
}

// Returns a set of all possible next nodes. Each of these
// should be allocated a new cursor.
std::set<Cursor> Cursor::next_cursors(
    const Token &_next_tok) const
{
    std::set<Cursor> out;
    auto cur = history.back().first;
    std::stack<ParseNode *> next_call_stack = call_stack;

    while (cur->type != NORMAL)
    {
        if (cur->type == CALL)
        {
            // Call node: Enter and push

            if (call_stack.size() >= max_cursor_recursion_depth)
            {
                throw std::runtime_error(
                    "Max recursion depth reached.");
            }

            next_call_stack.push(cur->get_exit_point());
            cur = cur->get_entry_point();
        }
        else if (cur->type == RETURN)
        {
            // Return node: Exit and pop

            if (next_call_stack.empty())
            {
                // Attempting to move beyond end of valid parse;
                // Eliminate cursor.
                return {};
            }

            cur = next_call_stack.top();
            next_call_stack.pop();
        }
    }

    for (const auto &p : cur->transitions)
    {
        if (boost::regex_match(_next_tok.str, p.first))
        {
            out.insert(Cursor(*this, p.second, _next_tok,
                              next_call_stack));
        }
    }

    return out;
}

bool Cursor::is_valid_end() const noexcept
{
    return (history.back().first->type == RETURN &&
            call_stack.empty());
}
