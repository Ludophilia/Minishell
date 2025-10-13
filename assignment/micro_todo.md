# Minishell

## 4/10 onwards

### Structure

- [o] Restructure and clean that function and core modules (main, interface)

- [x] Add clean init procedure and check if everything remain
fresh and cleaned from line to line...

- [ ] check if most modules are norm compliant

- [x] Restructure exec.c
	- [o] Improve waiting logic

- [x] Restructure parser
	- [x] especially parser_word.c which is very stuffy or confused
	- [x] Improve environment variable expansions
	- [x] test it... I hope everything still runs well.

### Niz integration

- [o] Sync variable logic with those of nezlock in minishell.h
	especially that environment variables

- [o] Remove duplicates (cleanup logic) and integrate the new stuff 
in signals and builtins

- [o] Check includes/

- [o] Better integrate what in core/
	- [o] Interface, main, utils
- [ ] Better integrate what in exec/
	- [ ] exec, exec_buitins

- [o] Add signals/ (signals_new)
	- [ ] Core shell need SIGINT (2, ctrl+c) and SIGQUIT (3, ctrl+\) disabled
	- [ ] Child shell need SIGINT (2, ctrl+c) and SIGQUIT (3, ctrl+\)
	back to default.
	- [ ] Heredocs seems to needs signal management as well

- [ ] Add builtins/ (builtins_new)
	- [ ] every builtins

### Project Wide Variables

- [x] Think about that global variable. What is its purpose?
	- [x] Subject says "Be careful. This global variable must only store the
	signal number and must not provide any additional information or access to data."


### Execution

- [o] set EXIT statuses for every situations (reds, signals, execution)

### Builtins

- [ ] increment SHLVL each time you open a minishell.

- [ ] Maybe improve here documents?
	- ^C processing with error message
	- why does readline sometimes add a new line and sometime not (^C)

- [ ] Maybe improve readline behavior
	- It really struggles with long lines... I don't know why.