# Builtins


## Export

### Examples

- `export`
export COLORTERM="truecolor"
export DBUS_SESSION_BUS_ADDRESS="unix:path=/run/user/110679/bus"
...
export ZSH="/home/jegerman/.oh-my-zsh"
export _="/usr/bin/bash"

- `export 42`
bash: export: `42': not a valid identifier
bash-5.1$ echo $?
1
bash-5.1$ printenv 42

- `export MINI`
bash-5.1$ echo $?
0
bash-5.1$ printenv MINI
bash-5.1$ 

- `export MINI=`
bash-5.1$ echo $?
0
bash-5.1$ env | grep MINI
MINI=
bash-5.1$

- `export MINI= SHELL= 42=`
bash-5.1$ echo $?
1

- `export MINI SHELL 42`
bash: export: `42': not a valid identifier

- `export MINI=$(pwd)`
bash-5.1$ echo $?
0
bash-5.1$ `printenv MINI`
/home/Documents/42/level3/Minishell

- `env | sort | wc -l`
57

- `export | sort | wc -l`
57