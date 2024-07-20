/*
Tests the ParseNode class.
*/

#include "../src/node.hpp"

// Pretty trivial testing here
int main()
{
    ParseNode n, entry, exit;

    n.set_as_return();
    assert(n.type == RETURN);

    n.set_entry_exit_points(&entry, &exit);
    assert(n.type == CALL);
    assert(n.get_entry_point() == &entry);
    assert(n.get_exit_point() == &exit);

    return 0;
}
