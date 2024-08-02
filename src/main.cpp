/*
Basic CLI frontend for parser testing.
*/

#define DEBUG

#include "cursor.hpp"
#include "lexer.hpp"
#include "load_grammar.hpp"
#include "parser.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

int main(int c, char *v[])
{
    // Assert well-formated CLI args
    if (c != 3)
    {
        std::cerr << "Plese provide 2 args: "
                  << "A source path and a grammar path.\n";
        return 1;
    }

    // Grab input filename and grammar file name
    const std::string source_path = v[1];
    const std::string grammar_path = v[2];
    Lexer l;

    // Load grammar
    Grammar g = load_grammar_file(grammar_path);

    // Create parser
    Parser p(g);

#ifdef DEBUG
    p.graphviz("main.dot");
    system("dot -Tpng main.dot -o main.png");
#endif

    // Initialize lexer on source file
    l.init_file(source_path);

    // Feed from lexer to parser
    while (!l.done())
    {
        Token t = l.next_token();
        if (t.str.empty())
        {
            break;
        }

        p.process_token(t);
    }

    try
    {
        // Get final parse path
        Cursor c = p.finalize();

        for (const auto &h : c.history)
        {
            std::cout << "In rule '" << h.first->rule_name
                      << "' at " << h.second.file << ":"
                      << h.second.line << "." << h.second.col
                      << " token: '" << h.second.str << "'\n";
        }
    }
    catch (std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';
        return 2;
    }

    std::cout << source_path << " matches grammar "
              << grammar_path << '\n';

    return 0;
}
