# Lexer

## What should be done?

- Emit tokens as you scan a command line.

- Which token should be emitted for:

### Normal cases (examples are very important, otherwise I can't solve this)

- `` (press enter) 
	- empty command, mangaged by readline.

- `tty`
- `                          tty`
	- 1 word.
- `/usr/bin/whoami`

- `ls -la`
- `"ls -la"`
	- 1 word.
- `'ls -la'`

- `git log -3 --oneline`
- `'git log -3' --oneline`

- `last | head -1`
- `last|head -1`
- `"last | head -1"`

- `< Makefile head -5`

- `< Makefile`
- `<Makefile`

- `> file1`
- `>file3`

- `>> file2`
- `>>file2`

- `<< END`
- `<<$USER`

- `$?`

- `"$?"`

- `echo $USER`

- `$`

- `echo $`

- `< Makefile cat | rev >> elifekaM`

### Strange cases like these:

- `                  ` (line with space and lines, readline level)

- `""` (word -> empty string but real command)
- `''`

- `"         "` (word -> spaces but real command)
- `'       '`

- `'` (expect more > next line)
- `"`

- `"'''"`
	- That's ONE word

- `last | ` (expect more with > next line)

- `"<"` (it's a word. Not the operator metacharacter value deactivated)

- `'<< END'` (it's still a word. And it will be interpreted as a command later)

### Or for erroneous lines like:

- `<` or `pwd <` (work with every other op: `<<`, `>>`, `>`)

- `|`. Not `pwd |` which works, PIPE_OP checks if there something before at
parsing phase.

### Or some EXTREME situations like these:

- `"ls'''''''''''''''''''''"" -la"`
	- That's ONE WORD!

... that's the goal.

## Why Tokenization?

- Maybe it **helps for the parsing phase.**

	-> Yes it does. **It is separation of concerns in action.**
		- Part ONE (lexing): we IDENTIFY the different PARTS/COMPONENTS of the
		command line: words, operators, EOL.

		- Part TWO (parsing): 
			- we FOCUS on the MEANING/ROLE of those parts - it's often a matter 
			of token position.
				- which one is the command (the first word outside operators
				except pipe?)
				- which one(s) are the arguments (the word(s) after a command
				upto operator or EOL),
				- Which word is an operand for `<, >, <<, >>`
			
			- AND Raise ERRORS at that level if needed.
				- REDIN_OP and no word after -> syntax error
				- Most operators and nothing after -> syntax error
				- PIPE_OP nothing before -> syntax error

		- Part ???: expansion.

## How to handle quotes?

### Some examples

- `"ls -la"`
	- `""` signal that `ls -la` should be viewed as ONE UNIQUE word.
	- Stripped: `ls -la`

- `'       '`
	- Same, one UNIQUE word, full of spaces
	- Stripped: `       `

- `'`
	- That's still ONE word, but it's limited by EOL rather than a matching `'`
	in tokenization phase. Parser will open something to get more - a multiline
	command.

- `'"'"`
	- That will be ONE word. One quote is unclosed, limited by EOF. Parser 
	will open something to get more.
	- Stripped: `"\nlol` if typed lol" once enter in > phase.

- `'"'lol`
	- That's ONE word. `"lol: command not found`
	- Stripped: `"lol`
	- That also means a word doesn't stop at the end of a quote.

### What do quotes do, especially in that context?

- Quotes are just a way to block the value of some or all metacharacters...
- This affects...

	- How words are processed
		- Usually, words are separated by space(s) or EOL.
			- ls -la is TWO WORDS. `ls` `-la`
				- ` ` can perform its function as a word separator.
			- "ls -la" however is ONE WORD. `ls -la` 
				- because spaces have lost their meaning as a 
			separator.

	- If operators are processed AS operators or not.
		- This affect `<`, `<<`, `|`, `>>`, `>`
			- `echo hey '>' james` prints `lol > james`, not writes hey in 
			a file called james.
			- `last '|' head`. Won't print the 10 first lines of last but
			last sessions of users `|` and `head`, if exist 

	- If `$` + `xxx` gets a special meaning or not.
		- `echo "$USER"` prints the user name
		- `echo '$USER'` prints $USER
		- HOWEVER...
			- `echo "$"USER` prints $USER. It seems that expansion can only 
			happen within the confines of the dquotes if the $xxx pattern are
			enclosed in them.

### Are quotes a special form of token?

- Let's leave them as part of the rest rather than as an explicit token
that would only contain only them anyway.

- However, **we do respect the effect of quotes in the lexing phase**. Meaning:
	- They do affect how the line is broken down into WORDS.
	- They also affect the emission of operator tokens.

- The outer quotes will be removed later, at a later phase.

### Which meta-characters are blocked with...

- With single quotes ''
	- ` `, `"`, `<`, `<<`, `>>`, `>`, `$`
	- BONUS: `*`, `&&`, `||`, `(`, `)`

- With double quotes ""
	- ` `, `<`, `<<`, `>>`, `>`
	- BONUS: `*`, `&&`, `||`, `(`, `)`
	- NOT: `$.*` ($ + characters)

## Expand the $ in this phase or not?

- Not here, separation of concerns. Here -> Identify the parts.

## Conclusion? Which tokens ?

- WORD,
- IRED_OP, (<)
- IRED_HD_OP, (<<)
- ORED_OP, (>)
- ORED_AP_OP, (>>)
- PIPE_OP, (|)
- EOL

- BONUS:
	- Later...
