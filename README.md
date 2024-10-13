
# Runtime GLR Parser in `C++`
Jordan Dehmel, 2024
jdehmel@outlook.com

## Overview

This is a library providing a runtime Generalized LR (GLR)
parser for `C++` This is a parser loadable by `C++`, not
something that parses `C++`. It loads grammars via `EBNF`
(either from a file or from a list of strings). It has a
built-in lexer which is **static** in that it does not adjust
based on the grammar's terminals; This may change in future
versions, but as for now you must overload the `Lexer` class to
customize tokens. GLR parsers allow more flexibility in parsing
than recursive descent: For instance, GLR parsers can recognize
ambiguous grammars. That being said, the parse time is $O(n^2)$,
which is suboptimal.

**Note:** This library does not allow left recursion in its
grammars.

## Building and Installation

To build an example locally, run `make` in this folder. To
install, run `make install` (as sudo).

## Usage

Once the library has been installed as detailed above, it is
used as follows.

```cpp
#include <string>
#include <glr/glr.hpp>

int main()
{
    const std::string source_path = "fizz.buzz";
    const std::string grammar_path = "foo.bar";

    // Load grammar
    Grammar g = load_grammar_file(grammar_path);

    // Create parser
    Parser p(g);

    // Initialize lexer on source file
    Lexer l;
    l.init_file(source_path);

    // Feed from lexer to parser
    while (!l.done())
    {
        Token t = l.next_token();

        // EOF token
        if (t.str.empty())
        {
            break;
        }

        // Feed this token to parser
        p.process_token(t);
    }

    // Get final parse path
    // This throws an error if the file did not parse or if the
    // parse was ambiguous.
    Cursor c = p.finalize();

    return 0;
}

```

This should be linked with the `-Lglr` flag.

## Testing

To run the unit tests, run `make test`.
