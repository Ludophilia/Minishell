# Minishell

## What you actually need

### Part 0

- [o] Data architecture

- [o] Project architecture

- [x] Makefile + Libft

### Part 1

- [x] Prompt + line read (readline).

- [o] Strong testing routine throughout the whole ordeal.

- [ ] Exit statuses exactly like bash where applicable.

- [ ] Env as your own list/map; keep it authoritative (don’t trust environ after start).

### Part 2

- [x] Lexer that handles quotes, escapes, operators: |, <, >, >>, <<.

- [o] Expansions: $VAR, $?, ~ (optional), with quote rules 
(no expand in single quotes; expand in double quotes; heredoc 
expands only when delimiter is unquoted).

- [o] Parser → AST (or a light pipeline/command list) with redirs & args
 attached to each command node.

### Part 3

- [ ] Executor: build the pipeline, set up redirs/dup2, heredocs, exec external
 commands or run builtins in-proc when no pipe.

- [ ] Signals: SIGINT interactive behavior (interrupt line, not exit), 
SIGQUIT ignored in parent, default in children.

### Part 4

- [ ] Builtins: 
	- echo -n, (no newline)
 	- cd, pwd, 
	- export, unset, env,
	- exit.
