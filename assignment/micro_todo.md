# Minishell

## 16/10 onwards

### Structure

- [ ] 🔴 check if most modules are norm compliant

### Signals

- [ ] Maybe improve here documents?
	- ^C processing with error message
	- why does readline sometimes add a new line and sometime not (^C)

### Exit status

- [ ] Manage exit status
	- 🔴 for redirections
		- especially errors
			- (< off, no such file or directory -> 1)

	- 🔴 for commands
		- [x] (oof: command not found -> 127)

	- for signals
		- `g_sig` should be the mechanism used to update signals `exit_status`

### Builtins

- [ ] increment SHLVL each time you open a minishell.

### Extra

- [ ] Maybe improve readline behavior
	- [ ] It really struggles with long lines... I don't know why.
