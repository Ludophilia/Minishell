# Execution

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

`> file1`
	- Create file1 in the current directory. There's is no program to run,
	why use a subshell to open that file?
	- returns 0

`> file1 > file2 <notfile1`
	- bash: notfile1: No such file or directory
	- Create file1 and file2 in the current directory. Fails to create
	notfile1.
	- returns 1

`> file1 > file2 <notfile1 | > file3`
	- bash: notfile1: No such file or directory
	- Create file1, file2 AND file3 in the current directory.
	- Fails to create notfile1 but does not stops creating file3 after that.
	- returns 0 (>file3)

- `> file1 > file2 <<END | <<OFF > file3`
	- Heredoc with END delimiter first read
	- Then heredoc with OFF
	- return 0

- `< fileNotFound sleep 10`
	- bash: oof: No such file or directory
	- return 1

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

	- later?
