#include "node.hpp"
#include <stdexcept>

void ParseNode::set_entry_exit_points(
    ParseNode *const _entry_point,
    ParseNode *const _exit_point) noexcept
{
    type = CALL;
    entry_point = _entry_point;
    exit_point = _exit_point;
}

void ParseNode::set_transitions(
    const std::list<std::pair<boost::regex, ParseNode *>>
        &_transitions) noexcept
{
    type = NORMAL;
    transitions = _transitions;
}

void ParseNode::merge_transitions(
    const std::list<std::pair<boost::regex, ParseNode *>>
        &_transitions) noexcept
{
    type = NORMAL;
    for (const auto &p : _transitions)
    {
        transitions.push_back(p);
    }
}

void ParseNode::set_as_return() noexcept
{
    type = RETURN;
}

ParseNode *const ParseNode::get_entry_point()
{
    if (type != CALL)
    {
        throw std::runtime_error(
            "Cannot fetch entry point of non-call parse node.");
    }

    return entry_point;
}

ParseNode *const ParseNode::get_exit_point()
{
    if (type != CALL)
    {
        throw std::runtime_error(
            "Cannot fetch exit point of non-call parse node.");
    }

    return exit_point;
}

const std::list<std::pair<boost::regex, ParseNode *>> &
ParseNode::get_transitions()
{
    if (type != NORMAL)
    {
        throw std::runtime_error("Cannot fetch transitions of "
                                 "non-normal parse node.");
    }

    return transitions;
}
