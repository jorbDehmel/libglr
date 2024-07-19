/*
Basic CLI frontend for parser testing.
*/

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

    // Load lexer and grammar objects
    Lexer l;
    Grammar g;
    load_grammar(grammar_path, l, g);

    // Create parser object
    Parser p(g);

    // Initialize lexer on source file
    l.init_file(source_path);

    try
    {
        // Feed from lexer to parser
        while (!l.done())
        {
            Token t = l.next_token();
            p.process_token(t);
        }

        // Get final parse tree
        Cursor parse_tree = p.finalize();
    }
    catch (std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';
        return 2;
    }

    return 0;
}
