# Parser

## What should be done now?

- Now, we have a fully functional array of tokens from a command line.

- Now we have to check:
	- if the tokens makes sense / if something is missing, raise an error 
	instead.
		- Operator token without WORD: `wow <`
		- unmatched quote in a WORD: e.g - `"salut`
		- trailing pipe (): e.g - `last |`
		- leading pipe: `| head -5`

	- and fill the structures with the data extracted from the command line
	via the tokens, data that will be necessary for executing command(s), 
	managing redirections...
		-  
		- expand the $ environment variables

## If the token makes sense

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