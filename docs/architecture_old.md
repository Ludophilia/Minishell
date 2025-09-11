# Software Architecture

## Components

* `main` is main.c, the module where the program starts as it contains
the main() function.
* `interface` is the module that manages everything related to the
command line interface (prompt(s), command line persistance and processing,
user events triggered with `ctrl`)
* `parser` is the module that analyzes a command line, break it down
(when needed) into significant components,*  `expander` is the module where
every word that starts with `$` (e.g. $HOME or $?) is replaced by its value.
* `cmdenv builder` is the module where every command and its
environment (files, pipes...) are built (in data).
* `cmd executor` is the module responsible for the actual execution
of the cmd(s) with their potential context (redirections / pipe) 
* `builtin executor` is the module responsible for the management
of the builtins like `echo` or `exit`
