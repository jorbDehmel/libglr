/*
Tests the Grammar class.
*/

#include "../src/cursor.hpp"
#include "../src/grammar.hpp"
#include "../src/lexer.hpp"
#include <iostream>

bool does_traverse(const std::list<std::string> &_input,
                   const Cursor &_cursor)
{
    std::set<Cursor> cursors = {_cursor};

    for (const auto &s : _input)
    {
        std::set<Cursor> new_cursors;

        for (auto cursor : cursors)
        {
            new_cursors.merge(cursor.next_cursors(s));
        }

        // std::cout << "After input " << s << ", there are "
        //           << new_cursors.size()
        //           << " viable parse paths.\n";

        cursors = new_cursors;
    }

    if (cursors.size() == 1)
    {
        std::cout << "Valid parse!\n";
        return true;
    }
    else
    {
        std::cout << "Invalid parse...\n";
        return false;
    }
}

int main()
{
    Lexer l;
    Grammar g;
    std::list<std::string> input;

    // Load grammar
    g.rules["main"] = {RuleNode(RuleNode::TERMINAL, "fizz"),
                       RuleNode(RuleNode::TERMINAL, "buzz"),
                       RuleNode(RuleNode::DISJUNCTION, "|"),
                       RuleNode(RuleNode::TERMINAL, "foo")};
    g.entry_rule = "main";

    // Compile loaded grammar
    GrammarGraph gg(g);
    gg.graphviz("test_grammar.dot");
    system("dot -Tpng test_grammar.dot -o test_grammar.png");

    // Initiate lexer with input file
    l.init_file("input/to_parse.txt");

    // Attempt parse
    assert(does_traverse({"fizz", "buzz"},
                         Cursor(gg.get_entry())));
    assert(does_traverse({"foo"}, Cursor(gg.get_entry())));
    assert(!does_traverse({"fizz", "fizz"},
                          Cursor(gg.get_entry())));

    return 0;
}
