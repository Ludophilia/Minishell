# Bonus

## What should be done?

### A real tester

- Build a real tester this time.
-- But how? What should be done?

### Command grouping `()`

- Implement command grouping management.
	- Executes commands bet in a subshell.
	- Affects redirections of the rightmost command
	- The subgroup have its own exit status
	- SIGINT kills the whole group

	- References:
		- https://www.gnu.org/software/bash/manual/html_node/Command-Grouping.html
		- https://pubs.opengroup.org/onlinepubs/009695299/utilities/xcu_chap02.html#tag_02_09_04

### List AND-OR `&&`, `||`

- Implement AND list or OR list management.
	- Execute commands that are on the right conditionaly to the exit status
	of the commands on the left.
	- `&&`: right command will be executed if left command returns 0.
	- `||`: left command will be executed if left command returns 
	something different from 0.
	- `&&` and `||` have the same precendence and are left associative

	- References:
		- https://www.gnu.org/software/bash/manual/html_node/Lists.html
		- https://pubs.opengroup.org/onlinepubs/009695299/utilities/xcu_chap02.html#tag_02_09_03
		
### Glob character `*`

- Implement the glob character 
	- Matches any string, including the null string
	- Not replaced if nothing matches it.

	- References :
		- https://www.gnu.org/software/bash/manual/html_node/Pattern-Matching.html
		- https://pubs.opengroup.org/onlinepubs/009695299/utilities/xcu_chap02.html#tag_02_13_02
		- https://en.wikipedia.org/wiki/Glob_(programming)