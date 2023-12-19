# Parser

## What should be done?

A sub program that take a line from the `interface` module and 
*make sense* of it...

## Which lines?

## LINE WITH ONLY ONE COMMAND

### Single command execution

#### Standard command execution (cmd)

- `cmd` `/path/to/cmd`
	- should execute the command or program at `/path/to/cmd`
	in a child process.
	- error code 0 (usually)

##### Errors (Standard command execution)

- `oof`
	- bash: oof: command not found
	- error code: 127

- `""`
	- bash: : command not found
	- error code: 127

- `/path/to/hell`
	- bash: /path/to/hell: No such file or directory
	- error code: 126

- `/path/to/heaven`
	- bash: /path/to/heaven: Permission denied
	- error code: 126

- `< /dev/random uwu`
	- bash: uwu: command not found
	- error code: 127

##### Weird cases to consider

- `echo 10 >>outfile3 11 12`
	- "10 11 12" is written in `outfile3`. It's like `echo 10  11 12` is
	what is being executed.
	- error code: 0

- `printf "GODDAMN!" >outfile1 >outfile2 >outfile3`
	- GODDAMN! is written to outfile3
	- outfile1 is created (is needed) but nothing is written to it.
	- outfile2 is created (is needed) but nothing is written to it.	
	- error code: 0

- `printf "GODDAMN!" >outfile1>outfile2>outfile3`
	- GODDAMN! is written to outfile3
	- outfile1 is created (is needed) but nothing is written to it.
	- outfile2 is created (is needed) but nothing is written to it.	
	- error code: 0

- `printf >outfile1>outfile2>outfile3 "GODDAMN!"`
	- GODDAMN! is written to outfile3
	- outfile1 is created (is needed) but nothing is written to it.
	- outfile2 is created (is needed) but nothing is written to it.	
	- error code: 0

- `printf >outfile1>outfile2>outfile3 "GOOD" "TO" "SEE" "YOU"`
	- GOOD is written to outfile3, likely following the execution of
	`printf "GOOD" "TO" "SEE" "YOU"`
	- outfile1 is created (is needed) but nothing is written to it.
	- outfile2 is created (is needed) but nothing is written to it.
	- error code: 0

- `echo 1 >outfile1 2 >outfile2 3 >outfile3`
	- 1 2 3 is written to outfile3.
	- outfile1 is created but nothing is written to it.
	- outfile2 is created but nothing is written to it.
	- error code: 0

- `echo 1 >>outfile1 2 >>outfile2 3 >>outfile3`
	- 1 2 3 is appended to outfile likely from `echo 10  11  12  `
	- outfile1 is created (if didn't exist) but nothing is written to it.
	- outfile2 is created (if didn't exist) but nothing is written to it.
	- error code: 0

### Redirection

#### Stdin redirection (< infile)

- `< infile cmd` `<infile cmd` `cmd < infile` `cmd <infile`
	- should execute the command or program at `/path/to/cmd` in a child
	process and redirect its stdin to infile.
	- error code: 0 (expected)

##### Errors (Stdin redirection)

- `<`
	- bash: syntax error near unexpected token `newline'
	- error code: 2

- `< </dev/random head`
	- bash: syntax error near unexpected token `newline'
	- error code: 2

- `<""`
	- bash: : No such file or directory
	- error code: 1

- `<lol`
	- bash: lol: No such file or directory
	- lol is not created
	- error code: 1

- `<lol<lel<lawl`
	- bash: lol: No such file or directory
	- lol is not created
	- lel or lawl neither
	- error code: 1

- `< oof tee`
	- bash: oof: No such file or directory
	- error code: 1

- `<lol<lel<lawl</dev/random head`
	- bash: lol: No such file or directory
	- lol is not created
	- lel or lawl neither 
	- /dev/random not open
	- error code: 1

#### Stdin redirection via heredoc (<< DELIMITER)

- `<< DELIMITER cmd` `<<DELIMITER cmd` `cmd << DELIMITER` `cmd <<DELIMITER`
	- should execute the command or program at `/path/to/cmd` in a child
	process and redirect its stdin to a temporary file that should end with
	DELIMITER.
	- Beware: The content of the heredoc shouldn't appear in the cmd history! (really, I see it in my bash...)
	- error code: 0

##### Errors (heredoc)

- `<<`
	- bash: syntax error near unexpected token `newline'
	- error code: 2

- `<< ""` `<< ''`
	- That's not an error. This will just make an empty chain the DELIMITER.

#### Stdout redirection (> outfile)

- `> outfile cmd` `>outfile cmd` `cmd > outfile` `cmd >outfile`
	- should execute the command or program at `/path/to/cmd` in a child
	process and redirect its stdout to outfile.

##### Errors (> outfile)

- `>`
	- bash: syntax error near unexpected token `newline'
	- error code: 2

- `> >outfile echo rofl`
	- bash: syntax error near unexpected token `>' (the second one)
	- outfile is not created
	- error code: 2



- `<""`
	- bash: : No such file or directory

- `<lol`
	- bash: lol: No such file or directory
	- lol is not created

- `<lol<lel<lawl`
	- bash: lol: No such file or directory
	- lol is not created
	- lel or lawl neither

- `< oof tee`
	- bash: oof: No such file or directory

- `< /dev/random uwu`
	- bash: uwu: command not found

- `<lol<lel<lawl</dev/random head`
	- bash: lol: No such file or directory
	- lol is not created
	- lel or lawl neither 
	- /dev/random not open


`> lol "echo lol"`
	- echo lol: command not found
	- creates lol
#### Stdout appending (>> outfile)

- `>> outfile cmd` `>>outfile cmd` `cmd >> outfile` `cmd >>outfile`
	- should execute the command or program at `/path/to/cmd` in a child
	process and redirect its stdout to outfile.

##### Errors (>> outfile)






### LINES WITH MULTIPLES COMMANDS

### PIPES

- ``
- ``

## Constraints

``

# Errors

### Redirections



`< oof tee | echo lol`
	- lol
	- bash: oof: No such file or directory


# Weird cases to consider



##