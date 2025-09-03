# Lexer

## What should be done?

Emit tokens as you scan a command line.

Which token should be emitted for:

### Normal cases (examples are very important, otherwise I can't solve this)

- `` (press enter) 
	- empty command, mangaged by readline.

- `tty`
- `/usr/bin/whoami`

- `ls -la`
- `"ls -la"`

- `git log -3 --oneline`
- `'git log -3' --oneline`

- `last | head -1`
- `last           |                head                 -1`
- `"last | head -1"`

- `< Makefile head -5`

- `< Makefile`

- `> file1`

- `>> file2`

- `<< END`

- `$?`

- `"$?"`

- `echo $USER`

- `$`

- `echo $`

- `< Makefile cat | rev >> elifekaM`

### Strange cases like theses:

- `                  ` (line with space and lines, readline level)

- `""` (word -> empty string but real command)
- `''`

- `"         "` (word -> spaces but real command)
- `'       '`

- `'` (expect more > next line)
- `"`

- `"'''"`

- `last | ` (expect more with > next line)

- `"<"` (it's a word. Not the operator metacharacter value deactivated)

- `'<< END'` (it's still a word. And it will be interpreted as a command later)

### Or for erroneous lines like:

- `<` or `pwd <` (work with every other op: `<<`, `>>`, `>`)

- `|`. Not `pwd |` which works, PIPE_OP checks if there something before at
parsing phase.

... that's the goal.

## Why Tokenization?

- Maybe it helps for the parsing phase.

	-> Yes it does. It is separation of concerns in action.
		- Part ONE (lexing): we IDENTIFY the different PARTS/COMPONENTS of the
		command line: words, operators, EOL.

		- Part ???(): expansion.

		- Part TWO (parsing): 
			- we FOCUS on the MEANING/ROLE of those parts - its often a matter 
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


## How to handle quotes?

### Are they a special form of token? Or should they be handled like a WORD?

- `"ls -la"`
	- `""` signal that `ls -la` should be viewed as ONE UNIQUE word.

- `'       '`
	- Same, one UNIQUE word, full of spaces

- `'`
	- That's still ONE word, but it's limited by EOL rather than a matching `'`
	in tokenization phase. Parser will open something to get more.

- `'"'"`
	- That's TWO words. One is limited by EOF.

### Should quotes be included in Token?

- Yes. Except if we allow two quotes token.

- That also means we will have to remove them in the expansion
or parsing phase.

## Expand the $ or not?

- Not here, separation of concerns.

## Conclusion? Which tokens ?

### Certainly

- WORD,
- OUTRD_OP,
- INRD_OP
- APPD_op
- HDOC_OP,
- PIPE_OP,
- EOL

### No

- single_q


- singleq_word?
- doubleq_word?
	-> Quoted words are just a way to block some metacharacters...
		- ls -la is TWO WORDS.
		- "ls -la" is ONE WORD because spaces have lost their meaning as a 
		separator.



