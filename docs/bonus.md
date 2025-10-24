# Bonus

## What should be done?

### A real tester

- Build a real tester this time.
-- But how? What should be done?

### Command grouping `()`

- Implement command grouping management.
	- Executes commands in a subshell.
	- Affects redirections of the whole command, generally the rightmost command.
	- The subgroup have its own exit status
	- SIGINT or SIGQUIT kill the subshell / the whole group, whatever 
	their progression.
	- `()` is a syntax error

	- References:
		- https://www.gnu.org/software/bash/manual/html_node/Simple-Commands.html
		- https://www.gnu.org/software/bash/manual/html_node/Pipelines.html
		- https://www.gnu.org/software/bash/manual/html_node/Lists.html
		- https://www.gnu.org/software/bash/manual/html_node/Command-Grouping.html
		- https://pubs.opengroup.org/onlinepubs/009695299/utilities/xcu_chap02.html#tag_02_09_04

	- Interactions with mandatory part:
		- quotes `'`, `"` deactivate `&&` and `||`, so beware with your lexer
		- parser will have to be reimplemented as an AST, preferrably a binary
		tree. (fits binary operations, short circuit evaluation)...
		- affect control flow of commands execution and pipelines and tree 
		design in the process.

	- Tree for `a && (b || c)`:
		- *letter* represent a command.

                                          AND
                                         /    \
                                        a    SUBSH
											   |    
										      OR    
                                            /    \
                                           b      c

### List AND-OR `&&`, `||`

- Implement AND list or OR list management.
	- Execute commands that are on the right conditionaly to the exit status
	of the commands on the left.
	- `&&`: right command will be executed if left command returns 0.
	- `||`: left command will be executed if left command returns 
	something different from 0.
	- `&&` and `||` have the same precedence and are left associative,
	meaning that in `echo a && echo b || echo c`; `echo b`, where the ambiguity
	lies as `&&` and `||` have the same precedence, will associate with the
	left operator first before processing the right one.
	- `&` is a syntax error as well... 

	- References:
		- https://www.gnu.org/software/bash/manual/html_node/Lists.html
		- https://pubs.opengroup.org/onlinepubs/009695299/utilities/xcu_chap02.html#tag_02_09_03

	- Interactions with mandatory part:
		- quotes `'`, `"` deactivate `&&` and `||`, so beware with your lexer
		- parser will have to be reimplemented as an AST, preferrably a binary
		tree. (fits binary operations, short circuit evaluation)...

	- Tree for `a && b | c | d || e`:
		- *letter* represent a command,
		- roughly equivalent to `(((a && b) | c) | d) || e)`. Parentheses are
		backed into the structure as a node.
		- Don't hesitate to flatten the structure for pipe execution.

                                           OR
                                         /    \
                                       PIPE    e
                                      /    \
                                     PIPE   d
                                    /    \
                                  AND     c
                                 /   \
                                a     b
 
### Glob character `*`

- Implement the glob character 
	- Matches any filepath string against the WHOLE pattern. So that 
	`echo Do*uments*_` will not match `Documents`.
	- Including the null string. In `echo Do*uments*****`, `Do*uments*****`
	matches Documents because the first `*` matches `c` perfectly and 
	every other `*****` matches NULL.
	- Not replaced if nothing matches it. So `echo z*` will just print z*
	- Sort the results in alphabetical order (look at your exam rank 3 for
	histogram based sort for example)
	- Use getcwd(), opendir(), readdir(), closedir(), maybe one of those
	stat() functions...

	- References :
		- https://www.gnu.org/software/bash/manual/html_node/Filename-Expansion.html
		- https://www.gnu.org/software/bash/manual/html_node/Pattern-Matching.html
		- https://pubs.opengroup.org/onlinepubs/009695299/utilities/xcu_chap02.html#tag_02_13_02
		- https://en.wikipedia.org/wiki/Glob_(programming)

	- Interactions with mandatory part:
		- quotes `'`, `"` deactivate them
		- env variable expansion do not really affect them but beware
			- echo $US*r will expand US and *r against the files in the current
			directory.
