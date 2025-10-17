# Minishell

## 16/10 onwards

### Structure

- [ ] check if most modules are norm compliant

## Execution

- [ ] 🔴 Restructure exec.c

- [ ] 🔴 Improve waiting logic
	- [ ] Improve waiting logic so that < /dev/urandom tail does not fail
	^C make the child defunct. The SIGINT processing logic need to be 
	implemented.
		- Managed with signals?

### Signals

- [ ] Maybe improve here documents?
	- ^C processing with error message
	- why does readline sometimes add a new line and sometime not (^C)

- [o] Add signals/ (signals_new)
	- [ ] Core shell need SIGINT (2, ctrl+c) and SIGQUIT (3, ctrl+\) disabled
	- [ ] Child shell need SIGINT (2, ctrl+c) and SIGQUIT (3, ctrl+\)
	back to default.

### Exit status

- [ ] Manage exit status
	- for redirections
		- especially errors
			- (< off, no such file or directory -> 1)

	- 🔴 for commands
		- [ ] (oof: command not found -> 127)

	- for signals

		- `g_sig` should be the mechanism used to update signals `exit_status`

### Project Wide Variables

- [x] Think about that global variable. What is its purpose?
	- [x] Subject says "Be careful. This global variable must only store the
	signal number and must not provide any additional information or access to data."


### Builtins

- [x] Repair echo

- [x] Look where the builtins write their data

### Environment

- [ ] increment SHLVL each time you open a minishell.

- [ ] Repair that faulty test
	- ```
	echo $PATH
	export PATH=TEST
	echo $PATH
	```

### Extra

- [ ] Maybe improve readline behavior
	- It really struggles with long lines... I don't know why.