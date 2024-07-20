/*
Tests the Parser class. Integration testing.
*/

#include "../src/lexer.hpp"
#include "../src/load_grammar.hpp"
#include "../src/parser.hpp"

bool does_traverse(Parser &_p,
                   const std::list<std::string> &_input)
{
    _p.reset();

    try
    {
        for (const auto &t : _input)
        {
            _p.process_token(t);
        }
        _p.finalize();
        return true;
    }
    catch (...)
    {
        return false;
    }
}

int main()
{
    Grammar g;
    Lexer l;

    load_grammar("input/grammar.txt", l, g);
    g.entry_rule = "MAIN";

    Parser p(g);
    p.graphviz("test_parser.dot");
    system("dot -Tpng test_parser.dot -o test_parser.png");

    assert(does_traverse(p, {"fizz", "foo", "bloop"}));
    assert(does_traverse(p, {"fizz", "blizz", "bop"}));
    assert(!does_traverse(p, {"fizz", "blizz", "bloop"}));

    return 0;
}
