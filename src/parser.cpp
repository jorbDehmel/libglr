/*
Implementations for the parser class.
*/

#include "parser.hpp"
#include "cursor.hpp"
#include <stdexcept>

Parser::Parser(const Grammar &_grammar)
    : g(_grammar), cursors({Cursor(g.get_entry())})
{
    cursors.clear();
    cursors.insert(Cursor(g.get_entry()));
}

void Parser::reset()
{
    cursors.clear();
    cursors.insert(g.get_entry());
}

void Parser::process_token(const Token &_what)
{
    std::set<Cursor> new_cursors;
    for (auto &c : cursors)
    {
        new_cursors.merge(c.next_cursors(_what));
    }

    cursors.clear();
    cursors = new_cursors;

    if (cursors.empty())
    {
        throw std::runtime_error(
            "Text does not match grammar.");
    }
}

Cursor Parser::finalize()
{
    if (cursors.size() > 1)
    {
        throw std::runtime_error(
            "A single parse tree could not be produced.");
    }
    else if (cursors.empty())
    {
        throw std::runtime_error(
            "Text does not match grammar.");
    }

    const auto out = *cursors.begin();
    if (!out.is_valid_end())
    {
        throw std::runtime_error(
            "Text begins, but does not finish, grammar.");
    }

    return out;
}
