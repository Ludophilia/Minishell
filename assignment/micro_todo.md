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
	- [ ] Improve environment variable expansions
	- [ ] test it... I hope everything still runs well.

### Project Wide Variables

- [ ] Think about that global variable. What is its purpose?
- [ ] Sync variable logic with those of nezlock in minishell.h
	especially that environment variables

### Execution

- [o] set EXIT statuses for every situations (reds, signals, execution)
