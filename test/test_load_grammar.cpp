/*
Tests the load_grammar function.
*/

#include "../src/load_grammar.hpp"
#include <iostream>

int main()
{
    Grammar g;
    Lexer l;

    load_grammar("input/grammar.txt", l, g);

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
