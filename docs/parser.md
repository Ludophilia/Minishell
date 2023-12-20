# Parser

## What should be done?

A sub program that take a line from the `interface` module and 
*make sense* of it...

## Which lines?

## **Line with only one command**

### Command execution (cmd)

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

- `< /dev/random uwu`
	- bash: uwu: command not found
	- error code: 127

- `/path/to/hell`
	- bash: /path/to/hell: No such file or directory
	- error code: 126

- `/path/to/heaven`
	- bash: /path/to/heaven: Permission denied
	- error code: 126

- `"echo lol"`
	- bash: echo lol: command not found
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

### Input Redirection (< <<)

#### Stdin redirection (<)

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
	- error code: 1
	- lol is not created

- `<lol<lel<lawl`
	- bash: lol: No such file or directory
	- error code: 1
	- lol is not created
	- lel or lawl neither

- `< oof tee`
	- bash: oof: No such file or directory
	- error code: 1

- `<lol<lel<lawl</dev/random head`
	- bash: lol: No such file or directory
	- error code: 1
	- lol is not created
	- lel or lawl neither 
	- /dev/random not open

#### Stdin redirection via heredoc (<<)

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

### Output Redirection (> >>)

#### Stdout redirection (>)

- `> outfile cmd` `>outfile cmd` `cmd > outfile` `cmd >outfile`
	- should execute the command or program at `/path/to/cmd` in a child
	process and redirect its stdout to outfile.

##### Weird Cases (> outfile)

- `>bruh.c`
	- bruh.c IS created / truncated
	- error code: 0

- `>lol>lel>lawl`
	- lol lel and lawl ARE created / truncated
	- error code: 0

- `</dev/random head -c 40 >lul>lol>lel`
	- lul lol and lel ARE created / truncated
	- error code: 0
	- the result of `head` is written to lel (the last redirected
	(>'d) file of the line)

- `</dev/random head -c 40 >1>2>3`
	- bash: syntax error near unexpected token `1' (interpreted roughly as
	 `</dev/random head -c 40 > > >`) (1> 2> Should not be managed so it's 
	not really an error in my case...)
	- error code: 2

##### Errors (> outfile)

- `>`
	- bash: syntax error near unexpected token `newline'
	- error code: 2

- `> > >`
	- bash: syntax error near unexpected token `>' (the second one)
	- error code: 2

- `> > > >four`
	- bash: syntax error near unexpected token `>' (the second one)
	- four is NOT created / truncated
	- error code: 2

- `> >outfile echo rofl`
	- bash: syntax error near unexpected token `>' (the second one)
	- error code: 2
	- outfile is NOT created / truncated

- `>""`
	- bash: : No such file or directory
	- error code: 1

- `>''`
	- bash: : No such file or directory
	- error code: 1

- `> forbidden` (a file with no permission)
	- bash: forbidden: Permission denied
	- error code: 1

- `>lol > ''`
	- bash: : No such file or directory
	- error code: 1
	- lol has been created / truncated

- `echo salut > '' >two`
	- bash: : No such file or directory
	- error code: 1
	- two is NOT created / truncated, seems the logic stopped after the
	parsing error.
	- NOTHING is written to two, seems the logic stopped after the
	parsing error.

- `echo salut > forbidden >two`
	- bash: forbidden: Permission denied
	- error code: 1
	- two is NOT created / truncated, seems the logic stopped after the
	parsing error.

- `echo salut >one >two > '' >four`
	- bash: : No such file or directory
	- error code: 1
	- one and two ARE created / truncated
	- four is NOT created / truncated, seems the logic stopped after the
	parsing error.
	- NOTHING is written to one or two, seems the logic stopped
	after the parsing error.

#### Stdout appending (>>)

- `>> outfile cmd` `>>outfile cmd` `cmd >> outfile` `cmd >>outfile`
	- should execute the command or program at `/path/to/cmd` in a child
	process and redirect its stdout to outfile.

##### Weird Cases (>> outfile)

- `>> bruh.c`
	- bruh.c IS created or not truncated if already exists
	- error code: 0

- `>>one>>two>>three`
	- one two and three ARE created or not truncated if already exists
	- error code: 0

- `</dev/random head -c 40 >>one>>two>>three`
	- one two and three ARE created or not truncated if already exists
	- error code: 0
	- the result of `head` is appended to three (the last redirected
	(>'d) file of the line)

- `</dev/random head -c 40 >>1>>2>>3`
	- bash: syntax error near unexpected token `1' (interpreted roughly as
	 `</dev/random head -c 40 >> >> >>`) (1> 2> Should not be managed so it's 
	not really an error in my case...)
	- error code: 2

##### Errors (>> outfile)

- `>>`
	- bash: syntax error near unexpected token `newline'
	- error code: 2

- `>> >> >>`
	- bash: syntax error near unexpected token `>' (the second one)
	- error code: 2

- `>> >> >> >>four`
	- bash: syntax error near unexpected token `>' (the second one)
	- four is NOT created / appended
	- error code: 2

- `>> >>outfile echo rofl`
	- bash: syntax error near unexpected token `>>' (the second one)
	- error code: 2
	- outfile is NOT created / appended

- `>>""`
	- bash: : No such file or directory
	- error code: 1

- `>>''`
	- bash: : No such file or directory
	- error code: 1

- `>> forbidden` (a file with no permission)
	- bash: : Permission denied
	- error code: 1

- `>>lol >> ''`
	- bash: : No such file or directory
	- error code: 1
	- lol IS created or not truncated if already exists

- `echo salut >> forbidden >>two` (a file with no permission)
	- bash: : Permission denied
	- error code: 1
	- two is NOT created or not truncated if already exists, seems the logic
	stopped after the parsing error.

- `echo salut >> '' >>two`
	- bash: : No such file or directory
	- error code: 1
	- two is NOT created or not truncated if already exists, seems the logic
	stopped after the parsing error.

- `echo salut >>one >>two >> '' >>four`
	- bash: : No such file or directory
	- error code: 1
	- one and two ARE created or not truncated if already exists.
	- four is NOT created / truncated, seems the logic stopped after the
	parsing error.
	- NOTHING is written to one or two, seems the logic stopped
	after the parsing error.

### **Line with only two or more commands**

### PIPES

- ``
- ``

- `< oof tee | echo lol`
	- lol
	- bash: oof: No such file or directory

## Constraints

``
