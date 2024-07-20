/*
Tests the Cursor class.
*/

#include "../src/cursor.hpp"
#include "../src/grammar.hpp"
#include <iostream>

void traverse(std::list<std::string> &_input,
              std::set<Cursor> &_cursors)
{
    for (const auto &s : _input)
    {
        std::set<Cursor> new_cursors;

        for (auto cursor : _cursors)
        {
            new_cursors.merge(cursor.next_cursors(s));
        }

        std::cout << "After input " << s << ", there are "
                  << new_cursors.size()
                  << " viable parse paths.\n";

        _cursors = new_cursors;
    }

    if (_cursors.size() == 1)
    {
        std::cout << "Valid parse!\n";
    }
    else
    {
        std::cout << "Invalid parse...\n";
    }
}

int main()
{
    ParseNode a, // Entry node
        b, c,    // Two branches off
        d,       // Call function e
        e, f,    // Function + return node
        g;       // Final node
    /*
    Valid parse paths:
    fizz foo bloop
    fizz bar bloop
    */

    a.set_transitions({{boost::regex("fizz"), &b},
                       {boost::regex("fizz"), &c}});

    b.set_transitions({{boost::regex("foo"), &d}});
    c.set_transitions({{boost::regex("bar"), &d}});
    d.set_entry_exit_points(&e, &g);

    e.set_transitions({{boost::regex("bloop"), &f}});
    f.set_as_return();
    g.set_as_return();

    graphviz("test_cursor.dot", {&a, &b, &c, &d, &e, &f, &g},
             &a);
    system("dot -Tpng test_cursor.dot -o test_cursor.png");

    std::list<std::string> input;
    std::set<Cursor> cursors;

    input = {"fizz", "foo", "bloop"};
    cursors = {Cursor(&a)};

    traverse(input, cursors);

    input = {"fizz", "bar", "bloop"};
    cursors = {Cursor(&a)};

    traverse(input, cursors);

    input = {"fizz", "fizz", "bloop"};
    cursors = {Cursor(&a)};

    traverse(input, cursors);

    return 0;
}
