#include "grammar.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include <format>
#include <stdexcept>

Grammar load_grammar(const std::list<Token> &_text,
                     const std::string &_token_filename)
{
    std::string cur_rule;
    bool is_in_rule = false;
    bool should_be_colon = false;
    Grammar g;

    // Interpret token stream
    for (auto it = _text.begin(); it != _text.end(); ++it)
    {
        Token t = *it;

        if (should_be_colon)
        {
            if (t.str != ":")
            {
                throw std::runtime_error(std::format(
                    "Unterminated rule '{}' at {}:{}.{}",
                    cur_rule, t.file, t.line, t.col));
            }
            else
            {
                should_be_colon = false;
            }
        }

        // EOF
        else if (t.str == "")
        {
            break;
        }

        // Begin rule
        else if (!is_in_rule)
        {
            cur_rule = t.str;
            is_in_rule = true;

            if (g.entry_rule == "")
            {
                g.entry_rule = cur_rule;
            }

            should_be_colon = true;
        }

        // End rule
        else if (t.str == ";")
        {
            is_in_rule = false;
        }

        // Continue rule
        else
        {
            if (t.str.size() > 2 && t.str.front() == '"' &&
                t.str.back() == '"')
            {
                g.rules[cur_rule].push_back(RuleNode(
                    RuleNode::TERMINAL,
                    t.str.substr(1, t.str.size() - 2)));
            }
            else if (t.str == "|")
            {
                g.rules[cur_rule].push_back(
                    RuleNode(RuleNode::DISJUNCTION, t.str));
            }
            else
            {
                g.rules[cur_rule].push_back(
                    RuleNode(RuleNode::RULE_NAME, t.str));
            }
        }
    }

    if (is_in_rule)
    {
        throw std::runtime_error(
            std::format("Unterminated rule '{}' at {}:{}.{}",
                        cur_rule, _text.back().file,
                        _text.back().line, _text.back().col));
    }

    return g;
}

Grammar load_grammar_file(const std::string &_path)
{
    std::list<Token> raw_grammar;
    Lexer l;
    Token t;

    // Lex
    l.init_file(_path);
    while (!l.done())
    {
        do
        {
            t = l.next_token();

            if (t.str.starts_with("#include "))
            {
                std::string filepath =
                    t.str.substr(10, t.str.size() - 11);

                l.push_state_and_load(filepath);
            }
        } while (t.str.starts_with("//") ||
                 t.str.starts_with("#"));

        raw_grammar.push_back(t);
    }

    return load_grammar(raw_grammar, _path);
}
