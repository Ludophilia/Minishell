/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execvp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 17:29:59 by jgermany          #+#    #+#             */
/*   Updated: 2024/01/03 19:36:57 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

typedef enum e_procedure
{
	ACT_NOT_PROCEED,
	ACT_PROCEED
}	t_pro;

int	exec_cmd(char *file, char **argv)
{
	pid_t	pid;
	int		wstatus;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0 && execvp(file, argv) == -1) 
		exit(EXIT_FAILURE);
	else if (pid != 0 && (wait(&wstatus) == -1
		|| (wstatus >> 8 & 0xFF) == EXIT_FAILURE))
		return (-1);
	return (0);
}

int	test_cmd(int *i, char *file, char **argv)
{
	if (i == NULL)
		return (-1);
	if (*i > 1)
		write(2, "\n", 2);
	if (exec_cmd(file, argv) == -1)
		dprintf(2, "(#%i: %s: KO)\n", (*i)++, file);
	else
		dprintf(2, "(#%i: %s: OK)\n", (*i)++, file);
	return (0);
}

void	tests_run(t_pro proceed, int *i)
{
	char	*file;
	char	**argv;

	if (!proceed)
		return ;
	/* Simple command to start: `echo hello`. The command: `echo`, its unique
	argument: `hello`. argv = ["echo", "hello", 0]. Whitespaces act as a 
	separator for commands and arguments.*/
	file = "echo";
	argv = (char *[]){"echo", "hello", 0};
	test_cmd(i, file, argv);
	/* Next example shows that argv[0] do not have to match the filename... */
	file = "echo";
	argv = (char *[]){"some_bullshit", "hello", 0};
	test_cmd(i, file, argv);
	/* So, do whitespace get removed in a command like `echo   hello`? Typing
	`echo   hello` prints `hello`, showing that the whitespaces between args
	are indeed removed... One way to keep them is to use single or double 
	quotes. */
	file = "echo";
	argv = (char *[]){"echo", "hello", 0};
	test_cmd(i, file, argv);
	/* A good question to ask is where is the command in something like 
	`git log -1 --oneline`... Turns out that the equivalent argv array is
	["git", "log", "-1", "--oneline", 0] and not ["git log", "-1", "--oneline",
	0]. `git log` is not a command. `log` is an arg to the `git` command. */
	file = "git";
	argv = (char *[]){"git", "log", "-1", "--oneline", 0};
	test_cmd(i, file, argv);
	/* This example shows how the command `ls -l -a -t` is processed. Yes,
	again, whitespaces acts as separators so argv is ["ls", "-l", "-a",
	"-t", 0] */
	file = "ls";
	argv = (char *[]){"ls", "-l", "-a", "-t", 0};
	test_cmd(i, file, argv);
}

void	quoted_tests_run(t_pro proceed, int *i)
{
	char	*file;
	char	**argv;

	if (!proceed)
		return ;
	/* Quoting disables the role of whitespaces as a separator. In the command
	`stat "file with whitespaces"`, `stat` is a command and `"file with
	whitespaces"` its unique argument. It's important to note that double 
	quotes do not appear in the argv, as they had been certainly trimmed by
	the parser at some moment or at another.
	This command will return `stat: cannot statx 'file with whitespaces':
	No such file or directory`. No double quotes in the name...*/
	file = "stat";
	argv = (char *[]){"stat", "file with whitespaces", 0};
	test_cmd(i, file, argv);
	/* If the filename is quoted, execvp will fail... So NO QUOTES on
	filenames, because `"echo"` is not the same as `echo`. As argv[0] has minor
	importance however, quotes can be put there no problem... */
	file = "\"echo\"";
	argv = (char *[]){"echo", "hello", 0};
	test_cmd(i, file, argv);
	/* As whitespaces get removed in a command like `echo   hello`, one way
	to keep them is by using whitespaces (`echo "   hello"`). In the argv
	array, the result will be processed as: ["echo", "   hello", 0]. */
	file = "echo";
	argv = (char *[]){"echo", "   hello", 0};
	test_cmd(i, file, argv);
	/* If quotes are trimmed by the parser at one moment or another, how to 
	keep quotes in argv? That the role of `\"`.
	This command will return `stat: cannot statx '"file with whitespaces"':
	No such file or directory`. No double quotes in the name... */
	file = "stat";
	argv = (char *[]){"stat", "\"file with whitespaces\"", 0};
	test_cmd(i, file, argv);
}

void	playground_run(t_pro proceed, int *i)
{
	char	*file;
	char	**argv;

	if (!proceed)
		return ;
	file = "echo";
	argv = (char *[]){"echo", "hello", "world", 0};
	test_cmd(i, file, argv);
}
// (3/01) Report

/* - Whitespaces acts as separators when it comes to argument processing. The 
	amount of space between command and args does not matter...
 		- The command `git log -1` is processed as the argv ["git", "log",
			"-1", 0]
 		- The command `echo hello     world    ` is processed as the argv [
			"echo", "hello", "world", 0]
		- 
	- Quotes blocks the separation semantics of the whitespaces
		- examples? (4/01) fuyu demo, aki demo, kimi ga zutto matteiru yo

		GODDDAAAAAAAAAAAAAAAAAAAAMMMMMN... This shit's BONKERS AF.

		- `echo hello     world`
			- prints `hello world`

		- `echo hello     world`
			- prints `hello world`

		- `echo"hello""world"`
			- echohelloworld: command not found
			- WTF?
			- Quotes are definitely not separators, whitespaces still are, even when
			quotes are present.
			
		- `echo "hello""world"`
			- prints `helloworld`
			- `"hello""world"` becomes one argument? There is one white spaces between
			the command and the argument for sure but...?????
			- 

		- `echo " hello""world" again`
			- prints ` helloworld again`
			- `" hello""world"` is still one arg, `again` another, the split is done
			on the whitespaces that are not within quotes.
			
		- `echo " hello""world"again`
			- prints ` helloworldagain`
			- One argument
			- Again, the split is done, on the whitespaces that are not within quotes.
*/

int	main(void)
{
	int	i;

	i = 1;
	tests_run(ACT_NOT_PROCEED, &i);
	quoted_tests_run(ACT_NOT_PROCEED, &i);
	playground_run(ACT_PROCEED, &i);
	return (0);
}
