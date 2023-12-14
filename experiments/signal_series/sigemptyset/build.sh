#! /bin/bash

NAME=$(basename $(pwd))
cc -Wall -Wextra -Werror -g3 -o $NAME $NAME.c