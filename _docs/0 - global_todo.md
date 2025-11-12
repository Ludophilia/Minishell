# Minishell

# Mandatory

## What you actually need

### Part 0

- [x] Data architecture

- [o] Project architecture

- [x] Makefile + Libft

### Part 1

- [x] Prompt + line read (readline).

- [o] Strong testing routine throughout the whole ordeal.

### Part 2

- [x] Lexer that handles quotes, escapes, operators: |, <, >, >>, <<.

- [x] Expansions: $VAR, $?, ~ (optional), with quote rules 
(no expand in single quotes; expand in double quotes; heredoc 
expands only when delimiter is unquoted).

- [x] Parser → AST (or a light pipeline/command list) with redirs & args
 attached to each command node.

### Part 3

- [o] Executor: build the pipeline, set up redirs/dup2, heredocs, exec external
 commands or run builtins in-proc when no pipe.

- [o] Signals: SIGINT interactive behavior (interrupt line, not exit), 
SIGQUIT ignored in parent, default in children.

- [ ] Exit statuses exactly like bash where applicable.

- [o] Env as your own list/map; keep it authoritative (don’t trust environ after start).

### Part 4

- [o] Builtins: 
	- echo -n, (no newline)
 	- cd, pwd, 
	- export, unset, env,
	- exit.

# Bonus

# 🧭 Minishell – Bonus Transformation Roadmap

Upgrade your mandatory Minishell into a bonus-compliant shell handling:
- Logical operators `&&` and `||`
- Subshells `( ... )`
- Mixed redirections and pipelines
- Proper exit status propagation and signal handling

## ✅ Phase 1 — Extend Grammar and Tokenization

### Lexer
- [ ] Add new token types: `TOK_AND`, `TOK_OR`, `TOK_LPAREN`, `TOK_RPAREN`
- [ ] Ensure multi-character operators (`&&`, `||`) are parsed as single tokens
- [ ] Handle operator precedence correctly: parentheses > pipes > logical operators
- [ ] Add syntax validation for invalid sequences and unclosed parentheses

## 🌳 Phase 2 — Create the AST

### Parser
- [ ] Update recursive grammar rules to support `&&`, `||`, and `()`.
	- [ ] Grammar rules in Bnf notation:
		line          ::= and_or
		and_or        ::= pipeline ( ('&&' | '||') pipeline )*
		pipeline      ::= command ( '|' command )*
		command       ::= simple_command | subshell
		subshell      ::= '(' line ')'

- [ ] Adapt parser functions to produce new AST node types (`AND`, `OR`, `SUBSHELL`)
- [ ] Ensure parentheses create proper subtree isolation

### Data Structures
- [ ] Extend `t_node_type` enum with `NODE_AND`, `NODE_OR`, `NODE_SUBSHELL`
- [ ] Each `t_node` stores: `type`, `left`, `right`, and `child` (for SUBSHELL only)

### Builder Logic
- [ ] Create helper functions: `new_binary_node()` and `new_unary_node()`
- [ ] Ensure `PIPE` nodes are left-recursive during parsing
- [ ] Add a flattening pass for pipelines into a list of commands

## ⚙️ Phase 3 — Extend the Executor

### Recursive Evaluator
- [ ] Implement a recursive `exec_node()` dispatcher handling all node types
- [ ] Add short-circuit logic for `AND` and `OR`
- [ ] Return each node’s exit status to its parent

### Subshell Execution
- [ ] Implement `exec_subshell()` using `fork()`, recursive execution, and `waitpid()`
- [ ] Isolate signal handling and environment for subshells

### Pipeline Handling
- [ ] Flatten nested `PIPE` nodes before execution
- [ ] Allocate all pipes before forking
- [ ] Use `dup2()` for stdin/stdout wiring
- [ ] Close unused FDs and wait for all children

## 🔗 Phase 4 — Integration and Validation

### Redirections
- [ ] Verify correct order and locality of redirections
- [ ] Restore STDIN/STDOUT after command completion

### Exit Status Propagation
- [ ] Update `$?` after full tree evaluation
- [ ] Ensure `SUBSHELL` returns child status correctly
- [ ] Verify correct short-circuit behavior for logical operators

### Signal Handling
- [ ] Ignore `SIGQUIT` in parent, restore prompt on `SIGINT`
- [ ] Pass signals normally to child processes and subshells
- [ ] Maintain consistent readline behavior

### Testing Matrix
- [ ] Test logic chaining, subshells, pipelines, and mixed operators
- [ ] Test redirections and heredocs combined with logic
- [ ] Validate Ctrl+C and Ctrl+\ behavior in nested contexts

## 🧼 Phase 5 — Polishing and Validation

- [ ] Add clear syntax error messages
- [ ] Validate memory and FD cleanup with valgrind
- [ ] Sort wildcard expansions for consistent output
- [ ] Compare behavior with Bash for compliance
- [ ] Maintain consistent naming and code organization

## Phase 8 - Extra Polish

### Signals

- [ ] Maybe improve here documents?
	- ^C processing with error message
	- Improve memory management on exit with SIGINT (rl_done as a signal to
	trigger readline internal cleaning?)
	- why does readline sometimes add a new line and sometime not (^C)

### Builtins

- [ ] increment SHLVL each time you open a minishell.

### Readline

- [ ] Maybe improve readline behavior
	- [ ] It really struggles with long lines... I don't know why.


## 🧠 Bonus Tips

- [ ] Keep a `mandatory/` backup branch
- [ ] Use `exec_node()` as the universal entry point
- [ ] Think DFS: each node returns an exit status upward
- [ ] Store signal and status info in a minimal global struct
- [ ] Refactor early to maintain modular design

**Estimated total:** 10–15 full working days (≈ 60–100 focused hours)  
**Goal:** a modular, bonus-compliant shell that evaluates any AST structure 
via recursion, short-circuit logic, and isolated subshell execution.
