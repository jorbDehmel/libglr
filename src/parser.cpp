/*
Implementations for the parser class.
*/

#include "parser.hpp"
#include "cursor.hpp"
#include <algorithm>
#include <stdexcept>

Parser::Parser(const Grammar &_grammar) noexcept
    : g(_grammar), cursors({Cursor(g.get_entry())})
{
    cursors.clear();
    cursors.insert(Cursor(g.get_entry()));
}

void Parser::reset() noexcept
{
    cursors.clear();
    cursors.insert(g.get_entry());
}

void Parser::process_token(const Token &_what) noexcept
{
    std::set<Cursor> new_cursors;
    for (auto &c : cursors)
    {
        new_cursors.merge(c.next_cursors(_what));
    }

    cursors.clear();
    cursors = new_cursors;
}

Cursor Parser::finalize()
{
    const auto viable = n_viable();

    if (viable > 1)
    {
        throw std::runtime_error(
            "A single parse tree could not be produced.");
    }
    else if (viable == 0)
    {
        throw std::runtime_error(
            "Text does not match grammar.");
    }

    return *std::find_if(
        cursors.begin(), cursors.end(),
        [](const Cursor &i) { return i.is_valid_end(); });
}

void Parser::graphviz(
    const std::string &_filepath) const noexcept
{
    g.graphviz(_filepath);
}

int Parser::n_viable() const noexcept
{
    return std::count_if(
        cursors.begin(), cursors.end(),
        [](const Cursor &c) { return c.is_valid_end(); });
}

const std::set<Cursor> Parser::get_cursors() const noexcept
{
    return cursors;
}
