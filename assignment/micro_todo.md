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

- [ ] Check includes/

- [ ] Better integrate what in core/
	- [ ] Interface, main, utils
- [ ] Better integrate what in exec/
	- [ ] exec, exec_buitins

- [ ] Add signals/ (signals_new)
	- [ ] signals
- [ ] Add builtins/ (builtins_new)
	- [ ] every builtins

- [ ] Remove duplicates (cleanup logic) and integrate the new stuff 
in signals and builtins

### Project Wide Variables

- [ ] Think about that global variable. What is its purpose?
- [ ] Sync variable logic with those of nezlock in minishell.h
	especially that environment variables

### Execution

- [o] set EXIT statuses for every situations (reds, signals, execution)

### Builtins

- [ ] increment SHLVL each time you open a minishell.

- [ ] Maybe improve here documents?
	- ^C processing with error message
	- why does readline sometimes add a new line and sometime not (^C)

- [ ] Maybe improve readline behavior
	- It really struggles with long lines... I don't know why.