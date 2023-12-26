#! /bin/bash

make -s -C $MINISHELL/libs/libft all clean
cc -Wall -Wextra -Werror -o ft_strtrim ft_strtrim.c -lft -L$MINISHELL/libs/libft