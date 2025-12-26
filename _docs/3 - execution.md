# Execution

# Mandatory Part

## Current State

- We have an array of t_cmds.
	- with each one:
		- an array of char * (command (path or shortcut) + args)
		- an array of ireds (input redirections)
		- an array if oreds (output redirections)

-  What's next?
	- Now we have to make sure the redirections and pipes are set and 
	commands are executed.

## Redirections

- Redirections: Understand where to open (parent / child) and how to open them
	- Write the code to manage each redirections with error management
		- '<<' Heredocs 
		- '<' Input redirects
		- '>' Output redirects with creation
		- '>>' Output redirect with append

#### Examples

- [x] `> file1`
	- Create file1 in the current directory. (if doesn't exist)
	- Truncate file 
	- There's is no program to run, why use a subshell to open that file?
	- returns 0

- [x] `>> file1`
	- Create file1 in the current directory. There's is no program to run,
	why use a subshell to open that file?
	- returns 0

- [x] `< Makefile`
	- Reads Makefile (if exists)
	- returns 0

- [x] `<< END`
	- Opens stdin to get the data...
	- returns 0

- [x] `> file1 | < Makefile`
	- Copies in `file1`
	- Open `Makefile`
	- returns 0

- [x] `> file1 | <<OFF`
	- Copies in `file1`
	- Open heredoc with limiter `OFF`
	- return 0

- [x] `> file1 > file2 <<END | <<OFF > file3`
	- Heredoc with END delimiter first read
	- Then heredoc with OFF
	- return 0

#### Errors examples

- [x] `< fileNotFound sleep 10`
	- bash: oof: No such file or directory
	- sleep 10 is NOT EXECUTED
	- return 1

- [x] `> file1 > file2 <notfile1`
	- bash: notfile1: No such file or directory
	- Create file1 and file2 in the current directory. 
	- Fails to create notfile1.
	- returns 1

- [x] `<notfile1 > file1 > file2`
	- bash: notfile1: No such file or directory
	- Fails to create  file1 and file2 in the current directory.
	- returns 1

- [x] `> file1 > file2 <notfile1 | > file3`
	- bash: notfile1: No such file or directory
	- Create file1, file2 AND file3 in the current directory.
	- Fails to create notfile1 but does not stops creating file3 after that.
	- returns 0 (>file3)

- [x] `<notfile1 > file1 > file2 | > file3`
	- bash: notfile1: No such file or directory
	- Fails to create  file1 and file2 in the current directory.
	- Create file3 in the current directory.
	- returns 0

#### Extreme examples

- [ ] `< Makefile cat > file1 | <file2 tac >file3 | < fileNotFound head > file4`
	- file1 is created (if needed) -> cat executed.
		- Makefile is copied to file1
	- file2 fails -> file3 not created ; cmd not executed.
	- fileNotFound fails -> file4 not created ; head not executed.

	- `bash: file2: No such file or directory`
	- `bash: fileNotFound: No such file or directory`
	- returns 1

// Please come back once you can execute commands.

- `< fileNotFound head | < ../Makefile cat > file41 >file42 >file43`
	- bash: fileNotFound: No such file or directory
		- that command group stop at `< fileNotFound`

	- The following command is still executed
	- file41, file42, file43 are created if needed
	- Makefile is copied to file43 (the last one)

	- returns 0

- `< fileNotFound1 < ../Makefile cat > file41 | < fileNotFound2 head`
	- bash: fileNotFound1: No such file or directory
	- bash: fileNotFound2: No such file or directory

	- `cat` is not executed, file41 is not created.
	- The subgroup stops at first error.

	- `head` is not executed either. 

	- returns 1

### Where to open?

- Where to open redirections files? Parent or child?
	
### I will go with the parent... But why?

#### It simplifies error management

- If there is an error in the redirection, there's no need to follow a
quite heavy procedure to create a child, execute it and wait for it in the
parent.
- It's way better to just exit early, avoid that overhead, and get ready
for the next command.

- `< fileNotFound sleep 10`, `sleep` won't be execve'd in a
child/subshell because the file `fileNotFound` doesn't exist. So why create
the environment in the first place.

#### It simplifies operations in EACH subshell/child.

Is it necessary to open a subshell for that command? Which redirection will
be one associated with STDIN or STDOUT? Which has precedence, the pipe
or the redirection?

- `> file1 > file2 > file3 ls -l`. Every file will be opened but only `file3`
will be associated to the output of `ls -l`.

- Wouldn't it be easier to just kill in the parent the closed
redirections and only send the right file (if valid) to the child 
who will just have to dup2 it?

- In `< fileNotFound1 sleep 10 | <fileNotFound2 sleep 10 | cat`, only 1
subshell `cat` is necessary as the first two commands will throw errors
in the the parent.

#### It allows us to centralize file / pipes processing and to better 
#### process their interactions (precedence rules).

- Every operation involving processing file descriptors for a program
are done once in bulk in the parent before childrens are spawned and executed,
which again simplify and centralize the whole process.

- That's even more interesting when we consider the link that exist between
pipes and redirections that requires some attention. It's better to 
treat that problem upstream in the parent, than to create anything downstream
to understand if a pipe has been opened in the parent and which one - the
pipe or the redirection - has precedence over which. 

- `< ../Makefile cat > file4 | head`
	- The output of cat is redirected to `file4`, 
	- NOT `stdout` via `head` and the pipe.

- `< ../Makefile cat | < ../docs/commands.md head`
	- The output of cat is redirected to the pipe, 
	- BUT `head` does NOT read the pipe but `commands.md`.

- `< ../Makefile cat > file41 | < fileNotFound head`
	- The output of cat is still redirected to `file4`, 
	- bash: fileNotFound: No such file or directory
	- return 1

- `< fileNotFound cat | < ../docs/commands.md head`
	- bash: fileNotFound: No such file or directory
	- BUT `head` does NOT read the pipe but `commands.md`.
	- return 1

#### Conclusion

- Let's do everything in the parent, it's cheaper, and there are some 
redirections that are precedence over others, so it's better and cheaper
to treat that problem upstream, where everything begins. 

- But there's a catch.

	- Opening redirections in the parents means we will have file descriptors
	Opened in the parent AND the child. So more work will have to be done
	here.

	- There are solutions like FD_CLOEXEC on `open`, but I haven't tested 
	them yet.

## Pipes

- Pipes: Understand where to open (parent / child) and how to open them.
	- Write the code to manage and open pipes (parent / child) with
	error management.

### Where to open?

#### Why the parents?

- It's impossible or at least very difficult to open pipes in the childs.
Fonctionally, pipes connect two programs together, so it's easier to 
set them in the process that connect those two programs together, the parent.

#### Conclusion

- Parents. I don't see where else they should be opened, as pipes are bridges...

## Commands

 == Understand how to build the pipeline of commands
 	with error management 
		- Write the code to find and execute the commands one by one with
		pipes and redirection correctly set on stdin and stdout.
		- Write how to execute the code to manage the builtins.

## Signals

	- later?

## Builtins

### Examples

```
$> export LOL="laughing out loud"
$> echo $LOL
laughing out loud
$> unset LOL | pwd
/home/jegerman/Documents/42/level3/Minishell
$> echo $LOL
laughing out loud
$> unset LOL
$> echo $LOL
```

```
>$ pwd
/home/jegerman/Documents/42/level3/Minishell
>$ cd .. | nl
>$ pwd
/home/jegerman/Documents/42/level3/Minishell
```

- In a pipeline, the builtins DO NOT modify the shell vars but the subshell

### Examples error

```
>$ pwd
/home/jegerman/Documents/42/level3/Minishell
>$ suriipu 1 | cd /
suriipu: command not found
>$ pwd
/home/jegerman/Documents/42/level3/Minishell
```

- In a pipeline, the builtins DO NOT modify the shell vars but the subshell.
Here cd / doesn't really do anything.

# Bonus part

## Current State

We have a functional AST... Now we have to manage the execution.

Let's see what should happen in execution for every node type.

(Inorder traversal??)

- AO: ANDOR node.
	- AND:
		- store left_code_exit_code in exit_code
			- How?
				- return value of the function that execute child?
				- pointer modified once the function has done executing
		
		- decide what to do with it:
			- exit_code == 0 ?
				- execute right_node
				- store left_code_exit_code in AO exit_code
			- exit_code != 0 ?
				- skip out right node and childrens entirely
	- OR:
		- store left_code_exit_code in exit_code
		- decide what to do with it:
			- exit_code == 0 ?
				- skip out right node and childrens entirely
			- exit_code != 0 ?
				- execute right_node
				- store right_code_exit_code in AO exit_code

- PI: PIPE node

	- Open a new pipe, the minute we pass on PIPE node (inorder traversal).
		- the left child will get pipe[1] as ofd.
		- the right child will get pipe[0] as ifd.

	- But how to manage a pipe node which has a pipe node as a child ?

						  PI(|) 
                         /     \
					   PI(|)   CMD(c)
					   /   \
				   CMD(a) CMD(b)

		- The parent PIPE node
			- left child (PIPE) will get pipeP[1] as ofd.
			- right child (CMD(c)) will get pipeP[0] as ifd.

		- The child PIPE node... (parent's left)
			- left child (CMD(a)) will get pipeC[1] as ofd
			- right child (CMD(c)) will get:
				- pipeC[0] as ifd.
				- pipeP[1] as ofd. (inheritance mechanism on right) 

	- But by the way... Should we open the pipe in the parent process or
	in child process?

		- Each command is executed in their own subprocess (after fork).
		This isolation allows execve to be processed without erasing the
		parent shell responsible for command line processing.

		- If pipes are opened in the subprocess... They will be isolated
		from the the rest of the command line. The opened fds won't be
		shared between the subs processes, so no interprocess communication
		will ever be possible.


- SUB: Subshell node
	- (echo a) | nl -> prints a with 1 before it.
                                 
								 PI(|)
								 /    \
				                SUB  CMD(nl)
								 |
						   CMD(echo a)

	- (echo a && echo b && echo c) | nl -> prints a, b, c, one line at a time
	with a number every line.
	- (echo a && echo b > /dev/null && echo c) | nl -> prints a, c, one line
	at a time with a number every line.

                               PI(|)
                             /      \
							SUB   CMD(nl)
							 |
						   AO(&&)
                          /     \
                       AO(&&)  CMD(echo c)
                      /     \
                CMD(echo a) CMD(echo b)

	- SUB has ofd that points to pipeP[1]... STDOUT in that context has been
	redirected / dup2'd to pipeP[1].

	That's exclusive to SUB, every command below SUB will have pipeP[1] as
	their ofd, instead of 1 / STDOUT. Fortunately for us, SUB is a subprocess,
	that means the redirection only affect that process and nothing else. 

24/12 - How do you manage

(echo a && echo b && echo c) > out

                             SUB <- Maybe I should add redirs to subshell node
							  |
                            AO(&&) 
						    /      \
					      AO(&&)  CMD(echo c)
						/       \     
			      CMD(echo a) CMD(echo b)

BY the way?

- CMD: CMD node
	- REDS
		- How do they interact with fds opened
			- cmd is executed 


################

Commands executed in the right context.
	- Pipes correctly opened
	- Redirections correctly opened

	== Opening the pipes / redirections
	== Managing the fds...
	== Finding the right command
