/*
Tests the load_grammar function.
*/

#include "../src/lexer.hpp"
#include "../src/load_grammar.hpp"
#include <iostream>

int main()
{
    Grammar g;
    Lexer l;

    g = load_grammar_file("input/grammar.txt");

    std::cout << "Rules:\n";
    for (auto &p : g.rules)
    {
        std::cout << "\t" << p.first << ":\t";
        for (auto &rn : p.second)
        {
            std::string s;

            if (rn.is_rule_name(s))
            {
                std::cout << '`' << s << "` ";
            }
            else if (rn.is_terminal(s))
            {
                std::cout << s << ' ';
            }
            else
            {
                std::cout << "| ";
            }
        }
        std::cout << '\n';
    }
    std::cout << "\nEntry rule: " << g.entry_rule << '\n';

    return 0;
}
