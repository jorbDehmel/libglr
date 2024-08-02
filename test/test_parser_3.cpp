/*
More tests of the parser class.
*/

#include "../src/load_grammar.hpp"
#include "../src/parser.hpp"
#include <iostream>
#include <list>

int main()
{
    std::cout << "Testing right recursion...\n";
    {
        Grammar g = load_grammar({Token("MAIN"), Token(":"),
                                  Token("\"a\""), Token("|"),
                                  Token("\"a\""), Token("MAIN"),
                                  Token(";")},
                                 "N/A");
        Parser p(g);
        p.graphviz("p3_right.dot");
        system("dot -Tpng p3_right.dot -o p3_right.png");

        p.process_token(Token("a"));
        p.process_token(Token("a"));
        p.process_token(Token("a"));
        p.finalize();
    }

    return 0;
}
