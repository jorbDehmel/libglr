#include "load_grammar.hpp"
#include "grammar.hpp"
#include "token.hpp"
#include <format>
#include <stdexcept>

void load_grammar(const std::string &_path, Lexer &_l,
                  Grammar &_g)
{
    std::string cur_rule;
    bool is_in_rule = false;
    Token t;

    // Lex
    _l.init_file(_path);

    // Interpret token stream
    while (!_l.done())
    {
        do
        {
            if (t.str.starts_with("#include "))
            {
                std::string filepath =
                    t.str.substr(10, t.str.size() - 11);

                _l.push_state_and_load(filepath);
            }

            t = _l.next_token();
        } while (t.str.starts_with("//") ||
                 t.str.starts_with("#"));

        // EOF
        if (t.str == "")
        {
            break;
        }

        // Begin rule
        else if (!is_in_rule)
        {
            cur_rule = t.str;
            is_in_rule = true;

            if (_g.entry_rule == "")
            {
                _g.entry_rule = cur_rule;
            }

            if (_l.done())
            {
                throw std::runtime_error(std::format(
                    "Unterminated rule '{}' at {}:{}.{}",
                    cur_rule, t.file, t.line, t.col));
            }

            t = _l.next_token();
            if (t.str != ":")
            {
                throw std::runtime_error(std::format(
                    "Unterminated rule '{}' at {}:{}.{}",
                    cur_rule, t.file, t.line, t.col));
            }
        }

        // End rule
        else if (t.str == ";")
        {
            is_in_rule = false;
        }

        // Continue rule
        else
        {
            if (_g.rules.contains(t.str))
            {
                _g.rules[cur_rule].push_back(
                    RuleNode(RuleNode::RULE_NAME, t.str));
            }
            else if (t.str == "|")
            {
                _g.rules[cur_rule].push_back(
                    RuleNode(RuleNode::DISJUNCTION, t.str));
            }
            else
            {
                _g.rules[cur_rule].push_back(
                    RuleNode(RuleNode::TERMINAL, t.str));
            }
        }
    }

    if (is_in_rule)
    {
        throw std::runtime_error(
            std::format("Unterminated rule '{}' at {}:{}.{}",
                        cur_rule, t.file, t.line, t.col));
    }
}
