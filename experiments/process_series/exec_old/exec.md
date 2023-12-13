# How to use execve instead of execvp? 

## What's the problem.

- The problem lies in the difference of first arg in execve and execvp.
	- For `int execve(const char *pathname, char *const argv[],
	char *const envp[])`, it's `pathname`, the path to the file to execute.
	- For `int execvpe(const char *file, char *const argv[],
	char *const envp[])`, it's `file`, a file that execvpe will search in
	every directory specified in the PATH environment variable,
	a the colon-separated list of directories, IF the specified filename
	DOES NOT contain a slash (/) character.

	- So...? We have to write something that will search PATH for the specified
	command if :

		- There is NO `/` in pathname (including for cmds that are in the 
		current PATH which are invoked via their full path, either via `./cmd` or `$PWD/cmd` or `/root/project/Pipex/cmd` by ex...)
		- and return the path to that command so that execve can execute it...

	- OK, and how to do it?

		- How to check if argv[2] contains `/` or not?
		- How do I get from PATH every dirs from where I need to search cmd ?
		- How to search for the file in every dir?
			- VERY SIMPLE ACTUALLY :
				- Take PATH from envp
				- Create a list of dirs from it with split it at ":"
				- Iterate on this list:
					- Combine the curr dir and the cmd to execute 
					- Test if the resulting path exist with access
						- return the resulting path if so
						- keep going if not
					- if not match, raise an error...