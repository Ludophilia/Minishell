#! /bin/bash

make -s -C $MINISHELL/libs/libft all clean
cc -Wall -Wextra -Werror -o ft_substr ft_substr.c -lft -L$MINISHELL/libs/libft