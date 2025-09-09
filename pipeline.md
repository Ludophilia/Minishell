# Pipeline

## Examples

### Input Redirections

- `< Makefile cat | head`
	- Makefile will be the one whose content will be redirected to `cat`

- `< file1 < Makefile cat | head`
	- Each file will be opened.
	- Makefile (the last one) will be the one whose content will be redirected to `cat`

- `< Makefile < file1 cat | head`
	- Each file will be opened.
	- file1 (the last one) will be the one whose content will be redirected to `cat`

- `< fileNotFound < Makefile cat | head | sleep 10 | sleep 20`
	- minishell: fileNotFound: No such file or directory
	- `head`, `sleep 10`, `sleep 20` are still executed in their subshell, in
	parallel. That would mean that file and pipes are not opened in the parent but... 
	- How are pipes managed then? They have to be opened in the parent.
	to be shared between childs.

- `<< END < Makefile cat | head`
	- Heredoc is opened and executed
	- Makefile (the last one) will be the one whose content will be redirected to `cat`

### Output Redirections

- `echo Hello Moto > file1`

- `echo hello again > file2 >> file3`
	- `file2` and `file3` are created if necessary
	- `hello again` goes in file3 (the last one)

- `echo one more time > file4 >> fileDenied`
	- `file4` is created if necessary
	- minishell: fileDenied: Permission denied

- `echo another one > file5 > file5 > file5`
	- `file6` is created if necessary
	- No Crash whatsoever...
	- cat file6 -> another one.