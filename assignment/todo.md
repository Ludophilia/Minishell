# 9 - Minishell

# I AM INEVITABLE (9th time!)

## Bootup (bip bip?)
 
- [x] Read the subject
- [x] Search everything you don't know, you don't understand. Cram
early, don't waste time, but try to understand as much as possible.
The rest will come as you're completing the assignment...
- [x] Start writing this todo list, relying on the assignment especially.

## Small experiments

- [o] There is a lot of NEW external functions in this project. Learn to use EVERY ONE of them.

	- [x] readline
	- [x] rl_clear_history
	- [x] rl_on_new_line,
	- [x] rl_replace_line
	- [x] rl_redisplay
	- [x] add_history,

	- [-] printf

	- [-] malloc
	- [-] free

	- [x] access
	- [x] unlink,
	- [x] open,
	- [x] close,
	- [x] read,
	- [x] write
	- [x] isatty,
	- [x] ttyname,
	- [x] getcwd,
	- [x] chdir,
	- [x] dup,
	- [x] dup2,
	- [x] pipe,

	- [x] fork,
	- [x] wait,
	- [x] waitpid,
	- [x] exit,
	- [x] wait3,
	- [x] wait4,
	- [x] execve,

	- [x] getenv,

	- [x] strerror,
	- [x] perror,

	- [ ] kill,
	- [ ] signal,
	- [o] sigaction,
	- [ ] sigaddset,
	- [ ] sigemptyset,

	- [x] stat,
	- [x] fstat,
	- [x] lstat,

	- [x] opendir,
	- [x] readdir,
	- [x] closedir,

	- [x] ttyslot,

	- [x] ioctl,
	
	- [o] tcgetattr,
	- [o] tcsetattr,

	- [o] tgetent,
	- [o] tgetflag,

	- [o] tgetnum,
	- [o] tgetstr,

	- [o] tgoto,
	- [o] tputs

## Makefile, main source file and includes

- [x] Write the project's Makefile

## Software Architecture 

- [x] Create a module view of the architecture. The goal of this is to have an
idea of what is composed minishell so that it's possible to divide the project
into modules that can be built in parallel (by multiple people).

- [x] Explain what the diagram mean...

## Development

### `main` module 

- [o] write main.c and keep improve it throughout the project

### `interface` module 

- [x] Start implementing error code related data 
structures: 0 (zero)
- [x] Manage error code at startup: 0 (zero)

- [x] Manage prompt (readline, infinite loop...)
- [o] Add the command history (add history / clear_history?)
	- [x] Make sure that 1 2 3 (empty line) (empty line) 4 5 returns
	5 4 3 2 1 as an history.
	- [ ] Why would someone need `clear history`?

- [x] Manage ctrl hotkeys in interactive mode
	- [x] manage ctrl + c (newline or SIGINT?)
		- [x] Make sure that the line buffer is reset every time
		SIGINT is sent. lol^C lol^C lol^C lel *enter* should be interpreted
		as sending the lel command..
		- [ ] MANAGE THE ERROR CODE! Error code: 130.
	- [x] manage ctrl + D (exit or EOF?)
	- [x] manage ctrl + \ (should do NOTHING)

### `parser` module 

- [ ] Extract information about command and files from the line
	- [ ] Manage to extract command + args
	- [x] Manage to extract file path for input redirection
	- [x] Manage to extract file path for output redirection
	- [x] Manage to extract LIMITER for here documents
	- [x] Manage to extract file path for output redirection in append mode

- [ ] Make transformations on the line parser to process `"` and `'`
	- [o] A `"` or `'` should make a ` ` (space) do nothing
	(in the context of a command or a path). With `"` or `'`
	around, ` ` is not a  separator or a terminator anymore...
	- [ ] A `'` should block every other metacharacter (`$`, `"`, `|`, `<`, `>`)
	interpretation.
	- [ ] A `"` should block every other metacharacter (`'`, `|`, `<`, `>`) interpretation except `$`...
	- [ ] A `'` or `"` that is not matched should raise an error.

- [ ] Manage pipes (`|`)
	- [ ] A pipe should signal two things:
		- [ ] the end of a command (potentially the start of a new one if
		there is smething beyond...) ;
		- [ ] a new entry in 
 		- [ ] the creation of a pipe

- [ ] Manage the parsing errors
	- (parser.md)

### `expander` module (part of `parser`?)

- [ ] Manage environment variables ($):
	- [ ] they should expand to their values or nothing if there is nothing

- [ ] Manage special variables ($):
	- [ ] handle ($?) the exit status of the recent executed foreground 
	pipeline

- [ ] Cleanout the string: remove those `"` and `'`

### `cmdenv builder` module 

- [ ] ???

### `cmd executor` module 

- [o] command searching (access, env, PATH)
- [o] pipe management
- [o] redirections
	- [o] manage input redirection (<) in every order
	- [o] manage output redirection (>) in every order
	- [o] manage append (>>) in every order
	- [o] manage heredoc (<<) in every order

- [ ] Should ctrl hotkeys (ctrl + d, ctrl + c, ctrl + \) be managed in 
that situation too?

### `builtin executor` module 

- [ ] manage `echo` (option -n)
- [ ] manage `cd` (relative (.././) and absolute)
- [ ] manage `pwd` ($PWD?)
- [ ] manage `export`
- [ ] manage `unset`
- [ ] manage `env`

- [x] manage `exit`
	- [x] Make sure nothing else than exit actually register

## Error management

- [ ] Too much file descriptors open at the same time! That's a disaster
waiting to happen...

## Development bonus

### 

- [ ] Expand the expander: Manage `*` expansion for the current working directory
- [ ] Expand the parser: manage `&&` operator
- [ ] Expand the parser: manage `||` operator
- [ ] Expand the parser: manage `()` operator for priorities
