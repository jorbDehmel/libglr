/*
This contains a graph representing all valid parse sequences,
with zero or more nodes marked such that they denote a valid
parse. It takes in a series of rules and compiles them into a
parse graph.
*/

#pragma once

#include "node.hpp"
#include <map>
#include <set>
#include <string>

/*
A single node in an uncompiled grammar. This can be a terminal /
literal, a rule name, or a disjunction operator. This generally
follows EBNF.
*/
class RuleNode
{
  public:
    enum Type
    {
        TERMINAL,
        RULE_NAME,
        DISJUNCTION
    };

    RuleNode(const Type _t, const std::string &_d)
        : type(_t), data(_d)
    {
    }

    inline bool is_terminal(std::string &_into) const
    {
        if (type == TERMINAL)
        {
            _into = data;
            return true;
        }
        return false;
    }

    inline bool is_rule_name(std::string &_into) const
    {
        if (type == RULE_NAME)
        {
            _into = data;
            return true;
        }
        return false;
    }

    inline bool is_disjunction() const
    {
        return type == DISJUNCTION;
    }

  private:
    Type type;
    std::string data;
};

/*
A rule in an uncompiled grammar; Simply a list of rule nodes.
*/
typedef std::list<RuleNode> Rule;

/*
A grammar is a set of one or more rules. Each rule has a name,
and one of them is somehow determined to be the entry rule.

A rule is a series of objects where each object is one of three
things: A terminal, the name of another rule, or a disjunction
operator. The end of a rule will be interpretted as a RETURN
node, the disjunction operator will split the path compiled, and
a rule name will denote a CALL node. Terminals are NORMAL nodes.
The way in which this Grammar object is constructed will not be
specified here, as different grammar representations may load
differently.
*/
class Grammar
{
  public:
    std::string entry_rule;
    std::map<std::string, Rule> rules;
};

/*
A compiled grammar. Note: Takes ownership of its nodes.
*/
class GrammarGraph
{
  public:
    GrammarGraph(const Grammar &_g);
    ~GrammarGraph();

    inline ParseNode *const get_entry() const noexcept
    {
        return entry;
    }

    void graphviz(const std::string &) const;

  private:
    std::set<ParseNode *> nodes;
    ParseNode *entry;

    inline ParseNode *new_node() noexcept
    {
        auto n = new ParseNode;
        n->set_as_return();
        nodes.insert(n);
        return n;
    }
};

/*
Save a graphviz (*.dot) representation of the given compiled
parse graph.
*/
void graphviz(const std::string &,
              const std::set<ParseNode *> &, ParseNode *const);
