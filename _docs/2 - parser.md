# Parser

## [Error Management]

### What should be done now?

- Now, we have a fully functional array of tokens from a command line.

- Now we have to check:

- if the tokens makes sense / if something is missing, raise an error 
instead.
	- Operator token without WORD: `wow <`
	- unmatched quote in a WORD: e.g - `"salut`
	- trailing pipe (): e.g - `last |`
	- leading pipe: `| head -5`
and fill the structures with the data extracted from the command line
via the tokens, data that will be necessary for executing command(s), 
managing redirections...
	-  
	- expand the $ environment variables

### Example of erroneous command lines

#### Syntax errors with operators

- `<` or `tac <`

- `<<` or `cat <<` or `cat <<      `

- `>` or `echo hello >` or `echo hello >     `

- `>>` or `git log --oneline >>`

- `|` or `pwd |` or `| fold -w 30`

#### Syntax errors with word

	- `"Word` or `"`
		- Unmatched quotes, bash does something to manage this, but the subject
		does not require it, so I will throw an error instead.

	- `'Weird` or `'`
		- 

## [Pipeline Creation]

### Where are we now?

- We have a clean list of tokens, now we have to... copy those tokens in the 
project structures, so we can execute commands.

#### We have no idea what the project structures look like.

##### Which of these tokens should be stored, where, and why?

- the WORD(s) that contains the command

	- the WORD(s) that indicate the path for IRED
		- IRED is not opened in the parent

	- the WORD(s) that indicate the delimiter for IRED_HD
		- IRED_HD is opened right after and stored in a pipe.

	- the WORD(s) that indicate the path for ORED 
		- ORED is not opened in the parent

	- the WORD(s) that indicate the path for ORED_AP
		- ORED_AP is not opened in the parent

So every TOK_WORD, whether it stands as its own or it is associated with
a redirection operator.

##### How to store all of these tokens?

- **Array of structures** that represents commands.

	- **char **command** for the WORD(s), expanded if necessary, cleaned from quotes.
		- TOK_PIPE -> next command
		- TOK_EOL -> no more command

	- One **array of struct redirect** for input redirections
		- The struct redirect:
			- a path, where word will be copied.
			- something to store the pipe
			- a type to know what should be done with the word.

	- One **array of struct redirect** for output redirections
		- ...

### Commands

- Commands are not just comprised of ONE word. Most of time, it's the command +
arguments with sometimes quotes and expansions in the mix.

- `echo context: $USER, via $SHELL at $PWD.`
	- the first word is `echo`, the command shortcut or path...
	- but `context:`, `$USER`, `via`, `$SHELL`, `at`, `$PWD` are still part of
	the command line.

- `$ECHO context: $USER, via $SHELL at $PWD.`
	- if `export ECHO='echo'`
	- $ECHO, the first word is still a the command and will have to be expanded
	to `echo`.

#### Weird cases

- `ls < ../Makefile -l > out -a > out2 -t`
- -> `[WORD] IRED_OP WORD [WORD] ORED_OP WORD [WORD] ORED_OP WORD [WORD]`

	- if we find a word - not preceded by a redirection operator, who 
	always consumes EXACTLY one WORD - it's very likely that this world is
	the start of a command.

	- However, that doesn't mean every next word will be part of the command,
	as we can see in that example.

	- Only `[WORD]` are a part of the command, the remainder are just
	redirections and their operators.

`$ECHO`

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

- `< Makefile <parser.md`
	- Does nothing (no command execution) if files exists
		- code 0
	- else error
		- code 1

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

- `> file1 > file2`
	- creates file1 and file2 if needed
	- error code 0.

- `> file1 > file2 | sleep 20`
	- creates file1 and file2 in the subshell if needed
	- no command execution, fail properly
	- sleep is executed in parallel
	- error code 0.


## [Quote cleanup / dollar expansion]

### What's the matter?

Each time we take a WORD, it's highly possible its content features...
	- OUTER single or double quotes
	- A symbol `$` followed by some characters

The goal of this intermediary step between Token Error Checking and
Pipeline Creation is to:
	- Create a new string from the WORD, usually via malloc().

	- Remove OUTER quotes, if they exists.
	- Expand the $ variable.

### Examples (basic)

- [x] `echo $USER`
	-> user_name (jegerman)

- [x] `echo $NOT_DECL_VAR`
	-> 

- [x] `echo $ USER`
	-> $ USER

- [x] `echo $USER$SHLVL$LANG`
	-> jegerman2en_US.UTF-8

- [x] `echo $USER?ORWHAT?`
	-> jegerman?ORWHAT
	-> An id is only comprised of letters, numbers, and underscores, 
	and begins with a letter or underscore.

### Examples (quoted)

- [x] `echo "$USER"`
	-> user_name (jegerman)

- [x] `echo '$USER'`
	-> $USER

- [x] `echo "$USER$SHLVL$LANG"`
	-> jegerman2en_US.UTF-8

- [x] `echo '$USER$SHLVL$LANG'`
	-> $USER$SHLVL

- [x] `echo "'$USER'"`
	-> 'jegerman'

- [x] `echo '"$USER"'`
	-> "$USER"

- [x] `echo "'''$USER'''"`
	-> '''jegerman'''

- [x] `echo '"""$USER"""'`
	-> """$USER"""

- [x] `echo "'123$SHIT'"`
	-> '123'
	-> SHIT is not a defined variable, so it expands to nothing
	-> equivalent to SHIT=""

### Examples (quoted $ sign)

- [x] `echo $`
	-> $

- [x] `echo "$"`
	-> $

- [x] `echo '$'`
	-> $

- [x] `echo '$' USER`
	-> $ USER

- [x] `echo "$" USER`
	-> $ USER

- [x] `echo '$'USER`
	-> $USER

- [x] `echo "$"USER`
	-> $USER

- [x] `echo "'$'USER"`
	-> '$'USER

- [x] `echo '"$"USER'`
	-> "$"USER

- [x] `echo "$"$USER`
	-> $jegerman

- [x] `echo '$'$USER$USER$SHLVL`
	-> $jegermanjegerman2

- [x] `echo "$"$USER$USER'$'SHLVL`
	-> $jegermanjegerman$SHLVL (minishell: jegermanjegerman$SHLVL)

- [x] `echo "$"'_USER'`
	-> $_USER

- [x] `echo "$""MINI"`
	- $MINI 

### Examples (not envv, here docs, out of scope)

- [x] `echo $%USER`
	-> $%USER

- [x] `<< $USER`
- [x] `<< '$USER'`
- [x] `<< "$USER"`
	-> Here document will use $USER as their terminator, every time.

- [ ] `echo "\"$\"USER"`
	-> "$"USER (minishell: out of scope)

- [ ] `echo $1USER?`
	-> USER (minishell: out of scope)
	-> An id is only comprised of letters, numbers, and underscores, 
	and begins with a letter or underscore.

### In which case a variable should be expanded 

- the name is legal: 
	- Beginning with a letter or underscore. ft_isapha() || '_'
	- A word consisting solely of letters, numbers, and underscores,  ft_isalnum() || '_'

- The variable name stops when:
	- i <= len
	- we meet something that is not  ft_isalnum() || '_'

## [Parent level redirection: Here document and pipes]

- Should we proceed here? YES <NO>

## BONUS

The parser now should be able to manage (), &&, ||.

We've seen that earlier, but what of the best ways to handle that is
building an AST as binary tree out of the token list.

How to handle this however?

### Examples of AST

- `echo a && echo b`
- `[WORD WORD] AND [WORD WORD]`

	What does structure the AST?
		- What does it mean?
			- How does the tree build "itself" programmatically.

			- We start from an array of tokens. How do we build a tree from this,
			programmatically?

`echo a && echo b`
	- Create a command via the tokens, create a node (CMD) out of it. That's the 
	root node?
	- && operator: Create a new node (&&), associate left part to last CMD token,
	(right part with already taken)


21/11 - Ahahaha... Still struggling on that shi.

RULES:

- line				::= and_or
- and_or			::= pipeline ( ('&&' | '||') pipeline )*
- pipeline			::= command ( '|' command )*
- command			::= simple_command | subshell
- subshell			::= '(' line ')'
- simple_command	::= word ( word | redirection )*
- redirection		::= ('<' | '>' | '<<' | '>>') word
- word				::= TOKEN

APPLICATION:

// 22/11 - Every top node type at least (and_or, pipeline, commands....) should
be represented in the Abstract parser tree, even if they don't . Why?
// ==  AST should match the grammar production rules
// ==  Being close to production rules make it more efficient to traverse, less branching...
// == Easier to debug, consistency makes it simpler...
// == That's how bash does it anyway.

SO PLEASE CORRECT WHAT'S BELOW, WE'RE GETTING CLOSE...


### What is the purpose of that AST?

	- Make the expr execution easier.
		- [But why?]
		- Binary branches match binary operations and help implement them.
			- AND: if left branch return 0, we can safely explore the right 
			one.
			- OR: if left branch return 0, we don't have to explore the right
			branch.
		- Parentheses are backed in the structure. Nodes with parentheses
		will be grouped. It's better to add a SUBSHELL node to make tree traversal and subshell mode easier to execute.


### 