#include "grammar.hpp"
#include "node.hpp"
#include <boost/regex/v5/regex_fwd.hpp>
#include <format>
#include <fstream>
#include <stdexcept>

GrammarGraph::GrammarGraph(const Grammar &_g)
{
    std::map<std::string, ParseNode *> rule_handles;

    // Build each rule
    for (const auto &_r : _g.rules)
    {
        auto name = _r.first;
        auto rule = _r.second;

        rule_handles[name] = new_node();
        auto cur_end = rule_handles[name];

        // Write rule from beginning to end
        for (auto node : rule)
        {
            std::string contents;
            if (node.is_rule_name(contents))
            {
                if (!rule_handles.contains(contents))
                {
                    throw std::runtime_error(std::format(
                        "Unknown rule '{}'", contents));
                }

                auto n = new_node();
                cur_end->set_entry_exit_points(
                    rule_handles[contents], n);
                cur_end = n;
            }
            else if (node.is_terminal(contents))
            {
                auto n = new_node();
                cur_end->set_transitions(
                    {{boost::regex(contents), n}});
                cur_end = n;
            }
            else if (node.is_disjunction())
            {
                cur_end = rule_handles[name];
            }
            else
            {
                throw std::runtime_error(
                    "Unknown rulenode type.");
            }
        }
    }

    // Set entrance rule
    entry = rule_handles[_g.entry_rule];
}

GrammarGraph::~GrammarGraph()
{
    for (const auto &node : nodes)
    {
        delete node;
    }
    nodes.clear();
}

void GrammarGraph::graphviz(const std::string &_filepath) const
{
    // Open file
    std::ofstream f(_filepath);
    if (!f.is_open())
    {
        throw std::runtime_error(
            "Failed to open file for graphviz.");
    }

    // Header
    f << "digraph {\n";

    // Node ids
    std::map<ParseNode *const, int> ids;
    for (const auto &node : nodes)
    {
        ids[node] = ids.size();
        f << '\t' << ids[node] << " [label=";

        if (node->type == CALL)
        {
            f << "\"CALL\"";
        }
        else if (node->type == RETURN)
        {
            f << "\"RET\"";
        }
        else
        {
            f << "\"\"";
        }

        f << "];\n";
    }

    // Edges
    for (const auto &node : nodes)
    {
        if (node->type == NORMAL)
        {
            for (const auto &p : node->transitions)
            {
                f << '\t' << ids[node] << " -> "
                  << ids[p.second] << " [label=\"" << p.first
                  << "\"];\n";
            }
        }
        else if (node->type == CALL)
        {
            f << '\t' << ids[node] << " -> "
              << ids[node->get_entry_point()]
              << " label=\"CALL\";\n"
              << '\t' << ids[node] << " -> "
              << ids[node->get_exit_point()]
              << " label=\"ON RETURN\";\n";
        }
    }
    f << "\tENTRY -> " << ids[entry] << ";\n";

    // Footer
    f << "}\n";

    // Close file
    f.close();
}
