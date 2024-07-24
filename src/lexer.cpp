#include "lexer.hpp"
#include <cstdint>
#include <cstdio>
#include <memory.h>
#include <stdexcept>

enum LexerState
{
    DELIM = 0,
    SINGLE_QUOTE,
    DOUBLE_QUOTE,
    SLASH,
    COMMENT,
    SINGLETON,
    OPERATOR,
    NUMERIC,
    ALPHABETIC,
    WHITESPACE,
    PREPROC,
    DUMMY,      // Always returns to DELIM on the next
    FINAL_STATE // This MUST be the last state listed.
};

const static uint64_t dfa_num_states = LexerState::FINAL_STATE;
const static uint64_t dfa_num_chars = UINT8_MAX;

static bool dfa_is_initialized = false;
static LexerState dfa[dfa_num_states][dfa_num_chars];

// Initialize the static DFA that the lexer uses for lexing
void initialize_dfa()
{
    // Constants
    const static std::string alphabet =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
    const static std::string numbers = "0123456789";
    const static std::string whitespace = " \t\n";
    const static std::string singletons = ";(){}[],?~";
    const static std::string operators = "!@#$%^&*-+=:<>|";

    // Default is delim
    memset(dfa, DELIM, sizeof(dfa));

    // Large blocks of characters via constants
    for (char c : alphabet)
    {
        dfa[DELIM][(size_t)c] = ALPHABETIC;
        dfa[ALPHABETIC][(size_t)c] = ALPHABETIC;
        dfa[NUMERIC][(size_t)c] = NUMERIC;
    }
    for (char c : numbers)
    {
        dfa[DELIM][(size_t)c] = NUMERIC;
        dfa[NUMERIC][(size_t)c] = NUMERIC;
        dfa[ALPHABETIC][(size_t)c] = ALPHABETIC;
    }
    for (char c : whitespace)
    {
        dfa[DELIM][(size_t)c] = WHITESPACE;
        dfa[WHITESPACE][(size_t)c] = WHITESPACE;
    }
    for (char c : singletons)
    {
        dfa[DELIM][(size_t)c] = SINGLETON;
    }
    for (char c : operators)
    {
        dfa[DELIM][(size_t)c] = OPERATOR;
        dfa[OPERATOR][(size_t)c] = OPERATOR;
        dfa[SLASH][(size_t)c] = OPERATOR;
    }

    // Misc. small changes
    dfa[DELIM]['.'] = SINGLETON;
    dfa[NUMERIC]['.'] = NUMERIC;
    dfa[DELIM]['/'] = SLASH;
    dfa[SLASH]['/'] = COMMENT;

    // Iterate over states
    for (int s = DELIM; s < FINAL_STATE; ++s)
    {
        if (s == DUMMY)
        {
            continue;
        }
        dfa[s]['#'] = PREPROC;
        dfa[s]['\''] = SINGLE_QUOTE;
        dfa[s]['"'] = DOUBLE_QUOTE;
    }

    for (uint8_t c = 0; c < dfa_num_chars; ++c)
    {
        if (c != '\n')
        {
            dfa[PREPROC][(size_t)c] = PREPROC;
            dfa[COMMENT][(size_t)c] = COMMENT;
            dfa[SINGLE_QUOTE][(size_t)c] = SINGLE_QUOTE;
            dfa[DOUBLE_QUOTE][(size_t)c] = DOUBLE_QUOTE;
        }
    }

    dfa[SINGLE_QUOTE]['\''] = DUMMY;
    dfa[DOUBLE_QUOTE]['"'] = DUMMY;
    dfa[SINGLE_QUOTE]['"'] = SINGLE_QUOTE;
    dfa[DOUBLE_QUOTE]['\''] = DOUBLE_QUOTE;
}

////////////////////////////////////////////////////////////////

Lexer::~Lexer()
{
    while (!include_stack.empty())
    {
        pop_state();
    }

    if (cur_state.f)
    {
        fclose(cur_state.f);
    }
}

void Lexer::init_file(const std::string &_path)
{
    if (!dfa_is_initialized)
    {
        initialize_dfa();
    }

    while (!include_stack.empty())
    {
        pop_state();
    }

    if (cur_state.f)
    {
        fclose(cur_state.f);
    }

    cur_state.filename = _path;
    cur_state.col = 0;
    cur_state.line = 1;
    cur_state.f = fopen(cur_state.filename.c_str(), "r");

    if (!cur_state.f)
    {
        throw std::runtime_error(
            "Failed to open file for lexing.");
    }
}

bool Lexer::done() const
{
    return (feof(cur_state.f) && include_stack.empty());
}

void Lexer::push_state_and_load(const std::string &_path)
{
    // Push current state
    include_stack.push(cur_state);

    // Load new state
    cur_state.filename = _path;
    cur_state.col = 0;
    cur_state.line = 1;
    cur_state.f = fopen(cur_state.filename.c_str(), "r");
}

void Lexer::pop_state()
{
    // Close current state
    if (cur_state.f)
    {
        fclose(cur_state.f);
    }

    // Load previous state from stack
    cur_state = include_stack.top();
    include_stack.pop();
}

Token Lexer::next_token()
{
    // Check if need to go up an include file
    // This is mostly safety checking
    while (feof(cur_state.f))
    {
        if (include_stack.empty())
        {
            throw std::runtime_error(
                "Cannot call next_token if done is true.");
        }
        else
        {
            // If this file is exhausted and there exists a file
            // higher on the include stack, load that one.
            pop_state();
        }
    }

    // Actual lexing here
    LexerState state = DELIM;
    Token out;
    char c;

    out.file = cur_state.filename;
    out.line = cur_state.line;
    out.col = cur_state.col;
    out.str = "";

    // If ignore_whitespace is true, eat all whitespace first
    if (ignore_whitespace)
    {
        while (true)
        {
            fread(&c, 1, 1, cur_state.f);

            if (feof(cur_state.f))
            {
                return out;
            }

            if (dfa[DELIM][(size_t)c] != WHITESPACE)
            {
                break;
            }

            // Update state
            if (c == '\n')
            {
                ++cur_state.line;
                cur_state.col = 0;
            }
            else
            {
                ++cur_state.col;
            }
        }

        fseek(cur_state.f, -1, SEEK_CUR);
    }

    while (true)
    {
        // Read next character
        fread(&c, 1, 1, cur_state.f);

        // Process in DFA
        state = dfa[state][(size_t)c];

        // If exhausted file, exit
        if (feof(cur_state.f))
        {
            return out;
        }

        // If need to break, do so and seek back 1 char
        else if (state == DELIM)
        {
            fseek(cur_state.f, -1, SEEK_CUR);
            break;
        }

        // Else, append to out.str
        out.str.push_back(c);

        // Update state
        if (c == '\n')
        {
            ++cur_state.line;
            cur_state.col = 0;
        }
        else
        {
            ++cur_state.col;
            if (c == '\\')
            {
                fread(&c, 1, 1, cur_state.f);
                out.str.push_back(c);
            }
        }
    }

    if (out.str.empty())
    {
        throw std::runtime_error(
            "Illegal lex of size 0 near " + cur_state.filename +
            ":" + std::to_string(cur_state.line) + "." +
            std::to_string(cur_state.col));
    }

    return out;
}
