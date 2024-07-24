/*
Tests the Lexer class.
*/

#include "../src/lexer.hpp"
#include <iostream>

int main()
{
    Lexer l;
    Token t;
    l.ignore_whitespace = false;

    l.init_file("../src/lexer.hpp");

    while (!l.done())
    {
        t = l.next_token();
        std::cout << "`" << t.str << "`\n";
    }
    std::cout << "\n\n";

    l.init_file("../src/lexer.cpp");

    while (!l.done())
    {
        t = l.next_token();
        std::cout << "`" << t.str << "`\n";
    }
    std::cout << '\n';

    return 0;
}
