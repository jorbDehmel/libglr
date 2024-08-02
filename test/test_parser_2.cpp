/*
More tests of the parser class.
*/

#include "../src/load_grammar.hpp"
#include "../src/parser.hpp"
#include <list>
#include <stdexcept>

int main()
{
    Grammar g =
        load_grammar({Token("MAIN"), Token(":"), Token("\"a\""),
                      Token("|"), Token("\"b\""), Token(";")},
                     "N/A");
    Parser p(g);
    bool flag;

    p.reset();
    p.process_token(Token("a"));
    p.finalize();

    p.reset();
    p.process_token(Token("b"));
    p.finalize();

    flag = false;
    p.reset();
    p.process_token(Token("c"));
    try
    {
        p.finalize();
    }
    catch (...)
    {
        flag = true;
    }
    if (!flag)
    {
        throw std::runtime_error(
            "Something parsed that shouldn't have");
    }

    return 0;
}
