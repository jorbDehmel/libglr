#include "grammar.hpp"
#include "node.hpp"
#include <boost/regex/v5/regex_fwd.hpp>
#include <format>
#include <fstream>
#include <stdexcept>

GrammarGraph::GrammarGraph(const Grammar &_g)
{
    std::map<std::string, ParseNode *> rule_handles;
    std::string contents;

    // Create roots (allows forward definitions)
    for (const auto &_r : _g.rules)
    {
        const auto name = _r.first;
        const auto root = new_node();

        root->rule_name = name;
        rule_handles[name] = root;
    }

    // Build each rule
    for (const auto &_r : _g.rules)
    {
        const auto name = _r.first;
        const auto rule = _r.second;
        auto root = rule_handles[name];
        auto cur_end = root;

        // Write rule from beginning to end
        for (auto node : rule)
        {
            if (cur_end == root && !node.is_terminal(contents))
            {
                throw std::runtime_error(std::format(
                    "Rule '{}' has one or more branches "
                    "beginning with non-terminal.",
                    name));
            }

            if (node.is_rule_name(contents))
            {
                if (!rule_handles.contains(contents))
                {
                    throw std::runtime_error(std::format(
                        "Unknown rule '{}'", contents));
                }

                const auto n = new_node();
                n->rule_name = name;

                cur_end->set_entry_exit_points(
                    rule_handles.at(contents), n);
                cur_end = n;
            }
            else if (node.is_terminal(contents))
            {
                const auto n = new_node();
                n->rule_name = name;

                cur_end->merge_transitions(
                    {{boost::regex(contents), n}});
                cur_end = n;
            }
            else if (node.is_disjunction())
            {
                cur_end->set_as_return();
                cur_end = root;
            }
        }

        // Append return node
        cur_end->set_as_return();
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
    ::graphviz(_filepath, nodes, entry);
}

void graphviz(const std::string &_filepath,
              const std::set<ParseNode *> &nodes,
              ParseNode *const entry)
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
              << " [label=\"CALL\"];\n"
              << '\t' << ids[node] << " -> "
              << ids[node->get_exit_point()]
              << " [label=\"ON RETURN\"];\n";
        }
    }
    f << "\tENTRY -> " << ids[entry] << ";\n";

    // Footer
    f << "}\n";

    // Close file
    f.close();
}
