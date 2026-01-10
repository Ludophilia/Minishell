# Parser

## What should be done?

A sub program that take a line from the `interface` module and 
*make sense* of it...

## Which lines?

## **Line with only one command**

### Simple command (cmd)

#### Simple command execution (cmd)

- `tty`
	- prints the user's tty
	- exit status: 0

- `/usr/bin/whoami`
	- prints the current user
	- exit status: 0

- `ls -la`
	- execute `ls` with the option `-la`
	- exit status: 0

- `which which`
	- prints the path of the `which` command
	- exit status: 0

- `/usr/bin/stat Makefile`
	- prints stats about Makefile file.
	- exit status: 0

- `git log -3 --oneline`
	- prints the last 3 commits, one line each
	- exit status: 0

- `bash -c "last -3"`
	- execute the `last` command in a subshell
	- exit status: 0

##### Errors (Standard command execution)

- `oof`
	- bash: oof: command not found
	- exit status: 127

- `""`
	- bash: : command not found
	- exit status: 127

- `< /dev/random uwu`
	- bash: uwu: command not found
	- exit status: 127

- `/path/to/hell`
	- bash: /path/to/hell: No such file or directory
	- exit status: 127

- `/path/to/heaven`
	- bash: /path/to/heaven: Permission denied
	- exit status: 126

- `"echo lol"`
	- bash: echo lol: command not found
	- exit status: 127

##### Weird cases to consider

- `echo 10 >>outfile3 11 12`
	- "10 11 12" is written in `outfile3`. It's like `echo 10  11 12` is
	what is being executed.
	- exit status: 0

- `printf >outfile1>outfile2>outfile3 "GODDAMN!"`
	- GODDAMN! is written to outfile3
	- outfile1 is created (is needed) but nothing is written to it.
	- outfile2 is created (is needed) but nothing is written to it.	
	- exit status: 0

- `printf >outfile1>outfile2>outfile3 "GOOD" "TO" "SEE" "YOU"`
	- GOOD is written to outfile3, likely following the execution of
	`printf "GOOD" "TO" "SEE" "YOU"`
	- outfile1 is created (is needed) but nothing is written to it.
	- outfile2 is created (is needed) but nothing is written to it.
	- exit status: 0

- `echo 1 >outfile1 2 >outfile2 3 >outfile3`
	- 1 2 3 is written to outfile3.
	- outfile1 is created but nothing is written to it.
	- outfile2 is created but nothing is written to it.
	- exit status: 0

- `echo 1 >>outfile1 2 >>outfile2 3 >>outfile3`
	- 1 2 3 is appended to outfile likely from `echo 10  11  12  `
	- outfile1 is created (if didn't exist) but nothing is written to it.
	- outfile2 is created (if didn't exist) but nothing is written to it.
	- exit status: 0

- `</dev/random head >one>two>three -c 40`
	- 40 bytes from /dev/random are written to three that is previously
	created if needed.
	- one, two are created or truncated but nothing is written to it.
	- exit status: 0

- `echo echo echo`
	- print `echo echo` on stdout
	- exit status: 0

### Input Redirection (< <<)

#### Stdin redirection (<)

- `< infile cmd` `<infile cmd` `cmd < infile` `cmd <infile`
	- should execute the command or program at `/path/to/cmd` in a child
	process and redirect its stdin to infile.
	- exit status: 0

- `<   /dev/random head -c 25`
	- executes `head` after redirected its `stdin` to `/dev/random`
	- exit status: 0

- `</etc/group head -1`
	- reads the first entry of `/etc/group`
	- exit status: 0

- `</etc/passwd                          tail -1`
	- reads the last entry of `/etc/passwd`
	- exit status: 0

##### Errors (Stdin redirection)

- `<`
	- bash: syntax error near unexpected token `newline'
	- exit status: 2

- `< </dev/random head`
	- bash: syntax error near unexpected token `newline'
	- exit status: 2

- `<""`
	- bash: : No such file or directory
	- exit status: 1

- `<lol`
	- bash: lol: No such file or directory
	- exit status: 1
	- lol is not created

- `<lol<lel<lawl`
	- bash: lol: No such file or directory
	- exit status: 1
	- lol is not created
	- lel or lawl neither

- `< oof tee`
	- bash: oof: No such file or directory
	- exit status: 1

- `<lol<lel<lawl</dev/random head`
	- bash: lol: No such file or directory
	- exit status: 1
	- lol is not created
	- lel or lawl neither 
	- /dev/random not open

- `</etc/passwd   <   /dev/random       < lol    <     bleh            tail -1`	

#### Stdin redirection via heredoc (<<)

- `<< DELIMITER cmd` `<<DELIMITER cmd` `cmd << DELIMITER` `cmd <<DELIMITER`
	- should execute the command or program at `/path/to/cmd` in a child
	process and redirect its stdin to a temporary file that should end with
	DELIMITER.
	- Beware: The content of the heredoc shouldn't appear in the cmd history!
	(really, I see it in my bash...)
	- exit status: 0

##### Errors (heredoc)

- `<<`
	- bash: syntax error near unexpected token `newline'
	- exit status: 2

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
	- exit status: 0

- `>lol>lel>lawl`
	- lol lel and lawl ARE created / truncated
	- exit status: 0

- `</dev/random head -c 40 >lul>lol>lel`
	- lul lol and lel ARE created / truncated
	- exit status: 0
	- the result of `head` is written to lel (the last redirected
	(>'d) file of the line)

- `</dev/random head -c 40 >1>2>3`
	- bash: syntax error near unexpected token `1' (interpreted roughly as
	 `</dev/random head -c 40 > > >`) (1> 2> Should not be managed so it's 
	not really an error in my case...)
	- exit status: 2

- `printf "GODDAMN!" >outfile1 >outfile2 >outfile3`
	- GODDAMN! is written to outfile3
	- outfile1 is created (is needed) but nothing is written to it.
	- outfile2 is created (is needed) but nothing is written to it.	
	- exit status: 0

- `printf "GODDAMN!" >outfile1>outfile2>outfile3`
	- GODDAMN! is written to outfile3
	- outfile1 is created (is needed) but nothing is written to it.
	- outfile2 is created (is needed) but nothing is written to it.	
	- exit status: 0

##### Errors (> outfile)

- `>`
	- bash: syntax error near unexpected token `newline'
	- exit status: 2

- `> > >`
	- bash: syntax error near unexpected token `>' (the second one)
	- exit status: 2

- `> > > >four`
	- bash: syntax error near unexpected token `>' (the second one)
	- four is NOT created / truncated
	- exit status: 2

- `> >outfile echo rofl`
	- bash: syntax error near unexpected token `>' (the second one)
	- exit status: 2
	- outfile is NOT created / truncated

- `>""`
	- bash: : No such file or directory
	- exit status: 1

- `>''`
	- bash: : No such file or directory
	- exit status: 1

- `> forbidden` (a file with no permission)
	- bash: forbidden: Permission denied
	- exit status: 1

- `>lol > ''`
	- bash: : No such file or directory
	- exit status: 1
	- lol has been created / truncated

- `echo salut > '' >two`
	- bash: : No such file or directory
	- exit status: 1
	- two is NOT created / truncated, seems the logic stopped after the
	parsing error.
	- NOTHING is written to two, seems the logic stopped after the
	parsing error.

- `echo salut > forbidden >two`
	- bash: forbidden: Permission denied
	- exit status: 1
	- two is NOT created / truncated, seems the logic stopped after the
	parsing error.

- `echo salut >one >two > '' >four`
	- bash: : No such file or directory
	- exit status: 1
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
	- exit status: 0

- `>>one>>two>>three`
	- one two and three ARE created or not truncated if already exists
	- exit status: 0

- `</dev/random head -c 40 >>one>>two>>three`
	- one two and three ARE created or not truncated if already exists
	- exit status: 0
	- the result of `head` is appended to three (the last redirected
	(>'d) file of the line)

- `</dev/random head -c 40 >>1>>2>>3`
	- bash: syntax error near unexpected token `1' (interpreted roughly as
	 `</dev/random head -c 40 >> >> >>`) (1> 2> Should not be managed so it's 
	not really an error in my case...)
	- exit status: 2

##### Errors (>> outfile)

- `>>`
	- bash: syntax error near unexpected token `newline'
	- exit status: 2

- `>> >> >>`
	- bash: syntax error near unexpected token `>' (the second one)
	- exit status: 2

- `>> >> >> >>four`
	- bash: syntax error near unexpected token `>' (the second one)
	- four is NOT created / appended
	- exit status: 2

- `>> >>outfile echo rofl`
	- bash: syntax error near unexpected token `>>' (the second one)
	- exit status: 2
	- outfile is NOT created / appended

- `>>""`
	- bash: : No such file or directory
	- exit status: 1

- `>>''`
	- bash: : No such file or directory
	- exit status: 1

- `>> forbidden` (a file with no permission)
	- bash: : Permission denied
	- exit status: 1

- `>>lol >> ''`
	- bash: : No such file or directory
	- exit status: 1
	- lol IS created or not truncated if already exists

- `echo salut >> forbidden >>two` (a file with no permission)
	- bash: : Permission denied
	- exit status: 1
	- two is NOT created or not truncated if already exists, seems the logic
	stopped after the parsing error.

- `echo salut >> '' >>two`
	- bash: : No such file or directory
	- exit status: 1
	- two is NOT created or not truncated if already exists, seems the logic
	stopped after the parsing error.

- `echo salut >>one >>two >> '' >>four`
	- bash: : No such file or directory
	- exit status: 1
	- one and two ARE created or not truncated if already exists.
	- four is NOT created / truncated, seems the logic stopped after the
	parsing error.
	- NOTHING is written to one or two, seems the logic stopped
	after the parsing error.


## **Line with some special characters**

### Double quotes ("")

#### Effect: Deactivate whitespaces

Everything within quote is treated as an SINGLE unit

Within a pair of `"`, ` ` (spaces) do nothing. They are not used as a separator
or a terminator for commands or file paths anymore...

#### Valid inclusions

- `"pwd"`
	- executes the command `pwd`
	- prints the working directory
	- exit status: 0

- `"last"`
	- executes the command `last`
	- prints the last sessions
	- exit status: 0

- `ls "-la"`
	- executes the command `ls` with the args
	- prints the list 
	- exit status: 0

- `> "Fate Apocrypha - 01.mkv" my_downloader`
	- creates the file `Fate Apocrypha - 01.mkv` if it does not exist
	- prints `my_downloader: command not found`
	- exit status: 127

#### Parser notes: again, whitespaces semantics are disabled

- `echo hello     world`
	- 2 args
	- prints `hello world`

- `echo "hello     world"`
	- 1 arg
	- prints `hello     world`

- `echo "\"hello     world\""`
	- 1 arg
	- prints `"hello     world"`

- `echo"hello""world"`
	- `echohelloworld: command not found`
	- 0 arg: WHAT THE FORK?
	- Quotes are definitely not separators, whitespaces still are, even when
	quotes are present.
	
- `echo "hello""world"`
	- prints `helloworld`
	- 1 arg: WHAT THE... PORK?
	- `"hello""world"` becomes one argument? There is one white spaces between
	the command and the argument for sure but...

- `echo " hello""world" again`
	- 2 args
	- prints ` helloworld again`
	- `" hello""world"` is still one arg, `again` another, the split is done
	on the whitespaces that are not within quotes.
 	
- `echo " hello""world"again`
	- 1 arg
	- prints ` helloworldagain`
	- Again, the split is done, on the whitespaces that are not within quotes.

#### Errors 

- `"echo echo echo"`
	- executes the command `echo echo echo`
	- echo echo echo: command not found
	- exit status: 127

- `"last | head -5"`
	- executes the command `last | head -5`
	- last | head -5: command not found
	- exit status: 127

- `"du -hd0 home #@f"`
	- executes the command `du -hd0 home #@f` (Mind the absence of /)
	- du -hd0 home #@f: command not found
	- exit status: 127

- `"du -hd0 $HOME"`
	- executes the file at `du -hd0 /home/#@f`. (Mind the /)
	- bash: du -hd0 /home/#@f: No such file or directory
	- exit status: 127

- `"./deez/nuts"`
	- executes the file at `./deez/nuts`. (Mind the /)
	- bash: ./deez/nuts: No such file or directory
	- exit status: 127

#### Effect: Everything is treated as a unit, spaces are preserved

- `echo "hello       world"`
	- prints `hello       world"` and not `hello world`,
	which is the result of `echo hello       world`
	- "hello       world" is ONE ARG to `echo`
	- Double quotes preserve the amount of spaces within a string.
	- exit status: 0

#### Effect: blocking most metacharacters

Enclosing characters in double quotes (‘"’) preserves the literal value
of all characters within the quotes, with the exception of ‘$’, ‘`’, ‘\’,
and, when history expansion is enabled, ‘!’. (Last one is not POSIX compliant)

NOTE: Only `$` should be managed in Minishell.

- `ls "srcs/*.c"`
	- ls: cannot access 'srcs/*.c': No such file or directory
	- The `*` character is not expanded to its value
	- exit status: 2

- `stat "$HOME"`
	- execute stat <path_to_home>
	- $HOME is expanded to its value
	- exit status: 0

- `"< /dev/random" head`
	- `<` does not mean redirection anymore
	- bash: < /dev/random: No such file or directory
	- exit status: 127

- `pwd "|" rev`
	- prints current directory absolute path
	- NOT in reverse, like it would have been done with `` 
	- exit status: 0

- `"<< LIMITER" tac`
	- prints `<< LIMITER: command not found` on stderr
	- This does not trigger here documents.
	- exit status: 127

#### Strange cases

- `ls "srcs/*.c`
	- if the quote is not closed on the same line, a prompt `>` will appear
	on the next line for completing the line. More line can be added until
	the delimiter `"` is entered alone on a line.
	- NOTE: Should NOT be managed by minishell.

### Single quotes ('')

#### Effect: everything within quote is treated as an SINGLE unit

- `'pwd'`
	- executes the command `pwd`
	- prints the working directory
	- exit status: 0

- `'last'`
	- executes the command `last`
	- prints the last sessions
	- exit status: 0

- `'echo echo echo'`
	- executes the command `echo echo echo`
	- echo echo echo: command not found
	- exit status: 127

- `'last | head -5'`
	- executes the command `last | head -5`
	- last | head -5: command not found
	- exit status: 127

- `'du -hd0 $HOME'`
	- executes the COMMAND `du -hd0 $HOME`.
	- du -hd0 $HOME: command not found
	- exit status: 127

- `'du -hd0 home #@f'`
	- executes the command `du -hd0 home #@f` (Mind the absence of /)
	- du -hd0 home #@f: command not found
	- exit status: 127

- `'./deez/nuts'`
	- executes the file at `./deez/nuts`. (Mind the /)
	- bash: ./deez/nuts: No such file or directory
	- exit status: 127

#### Effect: Everything is treated as a unit, spaces are preserved

- `echo 'hello       world'`
	- prints `hello       world"` and not `hello world`,
	which is the result of `echo hello       world`
	- "hello       world" is ONE ARG to `echo`
	- Double quotes preserve the amount of spaces within a string.
	- exit status: 0

#### Effect: blocking most metacharacters

Enclosing characters in single quotes (') preserves the literal value
of all characters within the quotes.

- `ls 'srcs/*.c'`
	- ls: cannot access 'srcs/*.c': No such file or directory
	- The `*` character is not expanded to its value
	- exit status: 2

- `stat '$HOME'`
	- stat: cannot statx '$HOME': No such file or directory
	- exit status: 1

#### Strange cases

- `'pw`
	- if the quote is not closed on the same line, a prompt `>` will appear
	on the next line for completing the line. More line can be added until
	the delimiter `'` is entered on a line.
	- Newlines will be added between every lines
	- NOTE: Should NOT be managed by minishell.

### Backlash (\\)

- NOTE: Should NOT be managed by your minishell.

- `echo \$PWD`
	- prints `$PWD`
	- exit code: 0

### Semi colon (;)

- NOTE: Should NOT be managed by your minishell.

- `whoami ; date ; tty`
	- prints user, date and terminal path on a different line
	- exit code: 0

### Dollar sign (`$`)

- `PIPE='|'`
- `pwd $PIPE rev`
	- prints current directory with no inversion whatsoever
	- exit code: 0

- `RBRACKET='>'`
- `pwd $RBRACKET outfile`
	- prints current directory with no redirection whatsoever
	- exit code: 0


### BONUS: Wildcards (*)

`*` matches any string, including the null string. NOTE: ONLY the current
working directory should be managed by your minishell.

- `echo *`
	- prints the files and folders in the current working directory as `*`
	matches everything into it. (not the hidden files)
	- exit status: 0

- `echo *e`
	- prints the files and folders in the current working directory that
	end with an `e`, including a file name `e`
	- Case sensitive
	- exit status: 0

- `echo a*`
	- prints the files and folders in the current working directory that
	starts with an `a`, including a file name `a`
	- Case sensitive
	- exit status: 0

- `echo * , * , *`
	- prints 3 times the list of files and folders in the current working
	directory, separating with `,`.
	- exit status: 0

#### Weird cases

- `echo ./*`
	- print the content of the current working directory (or that matches
	the pathname `$PWD/*`). Every file is formatted as: `./filename`.
	- exit status: 0

- `echo /*`
	- print the content of top level directory (or that matches
	the pathname `/*`). Every file is formatted as: `/filename`.
	- exit status: 0

#### Errors (Wildcards)

- `echo A*`
	- prints `A*` if there is no match
	- exit status: 0

## **Line with only two or more commands**

### Pipes (|)

A pipeline is a sequence of one or more commands separated by one
of the control operators `|`.

The output of each command in the pipeline is connected via a pipe to the 
input of the next command. That is, each command reads the previous command’s
output. 

This connection is performed before any redirections specified by `command1`.

- `echo salut | fold -w 1`
	- `salut` is written vertically on stdout
	- exit status: 0

- `pwd | rev`
	- prints the current 
	- exit status: 0

#### Errors (|)

- `|`
	- bash: syntax error near unexpected token `|'
	- exit status: 2

#### Weird Cases (|)

- `echo salut a tous |`
	-  a prompt `>` will appear on the next line for completing the command.
	Inputing an empty line will just generate a new one. The processing will 
	occur only if a line is not empty.

- `printf | echo salut`
	- `salut` on stdout
	- `printf: usage: printf [-v var] format [arguments]` on the 2nd line and
	on stderr instead on the pipe
	- exit status: 0 (the last command)

- `echo salut | printf`
	- `salut` on the read end of the pipe
	- `printf: usage: printf [-v var] format [arguments]` on stderr
	- exit status: 2 (the last command)

- `echo salut >one | head -c 80`
	- Nothing is printed on stdout 
	- The output of `echo salut` has been redirected to the file `one` instead
	of to the pipe.
	- exit status: 0

- `echo salut | < /dev/random head -c 40`
	- Print 40 bytes coming from `/dev/random` 
	- exit status: 0

### BONUS: AND List (&&) 

AND and OR lists are sequences of one or more pipelines separated by 
the control operators ‘&&’ and ‘||’, respectively.

An AND list has the form:

- `command1 && command2`
	- `command2` is executed if, and only if, `command1` returns an exit
	status of zero (success). 
	- The return status of AND and OR lists is the exit status of the 
	last command executed in the list.

- `sleep 5 && echo hello again`
	- `sleep 5` is executed and THEN 5s later, `hello again`
	- exit status: 0

- `sleep && echo hello again`
	- `sleep` is executed and nothing else as its exit status is not 0.
	- exit status: 1 (`sleep` exit status)

#### Errors

- `&&`
	- `bash: syntax error near unexpected token `&&'`
	-  exit status: 2

- `&`
	- `bash: syntax error near unexpected token `&'`
	-  exit status: 2

#### Weird Cases 

- `pwd &&`
	-  a prompt `>` will appear on the next line for completing the command.
	Inputing an empty line will just generate a new one. The processing will 
	occur only if a line is not empty.

### BONUS: OR List (||) 

AND and OR lists are sequences of one or more pipelines separated by 
the control operators ‘&&’ and ‘||’, respectively.

An OR list has the form:

- `command1 || command2`
	- `command2` is executed if, and only if, `command1` returns
	 a non-zero exit status.  
	- The return status of AND and OR lists is the exit status of the 
	last command executed in the list.

- `uwu 2> /dev/null || echo hello again`
	- `hello again` is printed on stdout as `uwu` is not a valid command.
	- exit status: 0

#### Errors

- `||`
	- `bash: syntax error near unexpected token `||'`
	-  exit status: 2

- `sleep 1 || echo hello again`
	- `sleep` is executed
	- and nothing else as its exit status is 0.
	- exit status: 0 (`sleep 1` exit status)

- `uwu || echo hello again` 🟥
	- `bash: uwu: command not found`
	- hello again is still printed, as uwu failure generated exit code 127.

### BONUS: Grouping Commands (())

#### Effect: Create subshells

- NOTE: Should not be implemented by your minishell?

- `(sleep 5)`
	- No subshell is created? (non visible in `ps`)
	- exit status: 0

- `(printf)`
	- No subshell is created? (non visible in `ps`)
	- exit status: 2

- `(exit 42)`
	- No subshell is created? (and yet the shell is not exited!)
	- exit status: 42

- `(sleep 5 | sleep 5)`
	- A subshell is created (PPID: your minishell PID)
	- The `sleep` processes are the child of the subshell
	- exit status: 0

- `sleep 5 | sleep 5`
	- No subshell is created
	- The `sleep` processes are the child of main shell
	- exit status: 0

- `(exec sleep 3)`
	- A subshell has to have been created (non visible in `ps`),
	otherwise the shell would have been killed once `sleep` returned.
	- exit status: 0


#### Effect: A group has an exit status which matches the last command executed

- NOTE: Should not be implemented by your minishell

- `sleep 7 | sleep 1 && echo END`
	- No subshell is created
	- `echo` will be executed 7 seconds later as its execution depends on
	the return value of the pipe
	- exit status: 0

- `(sleep 7 | sleep 3) && echo END`
	- A subshell is created (PPID: your minishell PID)
	- `sleep` and `sleep` are executed within that subshell.
	- `echo` will be executed 7 seconds later as its execution depends on
	the return value of the group
	- exit status: 0

- `(printf || echo FALLBACK)`	
	- A subshell is created (PPID: your minishell PID)
	- `printf` and `echo` are executed within that subshell.
	- `printf` status code != 0, that's why `echo` is executed 
	- exit status: 0

- `(echo START && printf)`	
	- A subshell is created (PPID: your minishell PID)
	- `printf` and `echo` are executed within that subshell.
	- `printf` status code != 0, that's why `echo` is executed 
	- exit status: 2

#### Effect: Modify priorities on `&&` `||`

##### With AND operator first (NO Impact if success)

- `sleep 5 && (exit 42) || echo FALLBACK`
	- `sleep` is executed for 5s
	- `exit` is executed then, as `sleep` returned 0
	- `echo FALLBACK` is executed then as `exit` returned 42.
	- exit code: 0

- `(sleep 5 && (exit 42)) || echo FALLBACK`
	- `sleep` is executed for 5s.
	- `exit` is executed then, as `sleep` returned 0.
	- `echo FALLBACK` is executed then as (`sleep` && `exit`) returned 42.
	- exit code: 0

- `sleep 5 && ((exit 42) || echo FALLBACK)`
	- `sleep` is executed for 5s
	- `exit` is executed then, as `sleep` returned 0
	- `echo FALLBACK` is executed then as `exit` returned 42.
	- exit code: 0

##### With AND operator first (Impact if failure)

- `(exit 42) && sleep 5 || echo FALLBACK`
	- `exit` is executed and returns 42.
	- `sleep` is NOT executed
	- FALLBACK is printed on stdout
	- exit code: 0

- `((exit 42) && sleep 5) || echo FALLBACK`
	- `exit` and `sleep` are executed in a subshell.
	- `exit` is executed and returns 42.
	- `sleep` is NOT executed
	- FALLBACK is printed on stdout
	- exit code: 0

- `(exit 42) && (sleep 5 || echo FALLBACK)` 🟥
	- `exit` is executed and returns 42
	- NOTHING ELSE is executed because `exit` returned 42.
	- exit code: 42

##### With OR operator first (Impact if success)

- `sleep 5 || echo FALLBACK && echo DONE`
	- `sleep` is executed for 5s seconds...
	- `echo FALLBACK` WILL NOT be executed as `sleep` returned 0.
	- `echo DONE` is executed
	- exit code: 0

- `(sleep 5 || echo FALLBACK) && echo DONE`
	- `sleep` is executed for 5s seconds...
	- `echo FALLBACK` WILL NOT be executed as `sleep` returned 0.
	- `echo DONE` is executed
	- exit code: 0

-  `sleep 5 || (echo FALLBACK && echo DONE)` 🟥
	- `sleep` is executed for 5s seconds...
	- `echo FALLBACK` and `echo DONE` WON'T be executed, as `sleep` returned 0.
	- exit code: 0

#### Effects On Output Redirections

- `echo LOL && echo LEL && echo LUL > lul`
	- LOL is printed on stdout
	- LEL is printed on stdout
	- LUL is redirected to lul
	- exit value: 0

- `(echo LOL && echo LEL && echo LUL) > lul`
	- LOL is redirected to lul
	- LEL is redirected to lul
	- LUL is redirected to lul
	- exit value: 0

#### Effects On Input Redirections

- `(head -2 && head -2 && head -2) < Makefile`
	- The first `head` displays the first 2 lines of Makefile
	- The second `head` displays the next 2 lines of Makefile
	- The second `head` displays the next 2 lines of Makefile
	- exit value: 0

#### Effects On Pipes

- `echo START | (rev | tr '[:upper:]' '[:lower:]') | cat -e`
	- Reverses the letters in START and make then lowercase. Add a $ as echo
	ends its output with a newline.
	- `rev` and `tr` are executed in a subshell
	- this subshell still shares the fds opened on the shell original,
	so communation still possible
	- exit status: 0

- `echo STARTED | (rev | tee) && echo MID | cat -e && echo END`
	- DETRATS is printed on stdout (`tee` is not connected to the pipe)
	- MID is sent through the pipe and outputted by `cat`
	- END is printed on stdout
	- exit status: 0

#### Effect: Affect the scope of variables

- NOTE: Should not be implemented by your minishell

#### Errors ()

- `()`
	- bash: syntax error near unexpected token `)'
	- exit status: 2

#### Weird Cases ()

- `echo LOL &&`
	-  a prompt `>` will appear on the next line for completing the command.
	Inputing an empty line will just generate a new one. The processing will 
	occur only if a line is not empty.
