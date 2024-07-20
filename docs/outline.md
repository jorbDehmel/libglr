## Object Definitions

### Rules

A `Rule` is an alias of a list of `RuleNodes`. This represents a
single rule in a grammar. Each node must be matched in order to
move onto the next one.

### RuleNodes

A single atomic item in a rule.

### Grammar

A `Grammar` is a set of rules along with an entry rule. The
entry rule represents a valid parse.

### ParseNode

A `ParseNode` object represents a single item in a
`GrammarGraph`. Each node is one of three types: Normal, call,
or return.

A **normal** node has no special conditions. Once a `Cursor`
is on a normal node, it only has to match one of the outgoing
`RegEx` pattern edges in order to advance. If no such pattern
can be found, the `Cursor` dies.

A **call** node is a special case. When a `Cursor` lands on a
call node, it immediately jumps to some "entry" node and pushes
some "exit" nodes onto its call stack. This movement is **not**
consumptive to the input stream. If the entry node is another
non-normal node, this process is repeated before ever looking at'
the input token stream.

A **return** node is another special case. When a `Cursor` lands
on such a node, it immediately jumps to the node on top of its
call stack (popping it off the stack). If the stack is empty,
the `Cursor` dies.

### GrammarGraph

A digraph of `ParseNode` objects which `Cursor` objects traverse
through.

### Cursor

A `Cursor` traverses a `GrammarGraph`, starting from some given
initial node. On each step, each `Cursor` does the following:

1) While the current `ParseNode` is not normal:
    - If the current node is a call node, jump to its entry node
        and push its exit node to the cursor's call stack
    - Else if the current node is a return node, pop and jump
        from the call stack
2) Iterate over outgoing `RegEx` edges from this `ParseNode`:
    For each edge that matches, spawn a child `Cursor` on the
    corresponding node.

A valid parse is represented by exactly one surviving `Cursor`
which has an empty call stack and is on a return node.

### Parser

A `Parser` object abstracts away the `Cursor` iteration on a
`GrammarGraph` object.
