# Shell Scripting Operators for Pipex

## Input redirection (<)

### Syntax

`< file cmd`
`cmd < file`

the blanks between `<` and the `file` and/or the `command` are optional.

### Description

The `<` operator redirect the standard input of the command `cmd` from `stdin`
to a file.  

### Usage

- `< /dev/urandom tee` makes `tee` get its input from `/dev/random` instead of a
stdin (the data sent to the command after the terminal has done listening to the
users input). `/dev/random` is a character pseudo device file that streams 
random data.

## Output redirection (>)

### Syntax

`> file cmd`
`cmd > file`

the blanks between `>` and the `file` and/or the `command` are optional.

### Description

The `>` operator redirect the standard output of the command `cmd` from `stdout`
to a file. 

The file will be created if it does not already exist, or will be overwritten
if it already does.

### Usage

- `ls > files` will make `ls` redirect its output to the file `files`. If
`files` does not already exist, it will be created, else its content will be
overwritten.

## | (pipe operator)

### Syntax

`cmd1 | cmd２`

The blanks around the `|` are optional. 

### Description

The `|` operator redirect the output of the command `cmd1` to the standard
input of `cmd2`.

It is equivalent to running:

`cmd1 > tmp`
`< tmp cmd2`
`rm tmp`

Note that cmd2 and cmd1 are not aware of each other, they just execute 
concurrently with the data they are given to. Piping is managed by the shell,
not the commands.

If a pipeline is initiated with `&`, all processes are still executed 
concurrently and the `&` applies to the whole pipeline.

### Usage

`printf "Hello %s\n" $USER | fold -w 5`. The output of `printf` is formatted
to be 5 cols wide thanks to `fold` who has taken its input from printf.
