# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/30 13:02:44 by jgermany          #+#    #+#              #
#    Updated: 2023/12/14 13:43:35 by jgermany         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 				:= libft.a

LIB 				:= ft
INCDIR 				:= includes
LIBDIR 				:= .

CC 					:= cc
CCFL 				:= -Wall -Wextra -Werror

DIR_CHAR			:= ./srcs/characters
DIR_FIL				:= ./srcs/files
DIR_INT				:= ./srcs/integers
DIR_MEM				:= ./srcs/memory
DIR_STR				:= ./srcs/strings
DIR_LST				:= ./srcs/lists
DIR_PF				:= $(DIR_STR)/printers/ft_dprintf

LFT_HDR				:= ./includes/libft.h

SRCS_MAN			:= $(DIR_CHAR)/classifiers/ft_isalnum.c
SRCS_MAN			+= $(DIR_CHAR)/classifiers/ft_isalpha.c
SRCS_MAN			+= $(DIR_CHAR)/classifiers/ft_isascii.c
SRCS_MAN			+= $(DIR_CHAR)/classifiers/ft_isdigit.c
SRCS_MAN			+= $(DIR_CHAR)/classifiers/ft_isprint.c
SRCS_MAN			+= $(DIR_CHAR)/converters/ft_tolower.c
SRCS_MAN			+= $(DIR_CHAR)/converters/ft_toupper.c
SRCS_MAN			+= $(DIR_CHAR)/printers/ft_putchar_fd.c

SRCS_MAN			+= $(DIR_FIL)/misc/get_next_line.c
SRCS_MAN			+= $(DIR_FIL)/misc/get_next_line_utils.c

SRCS_MAN			+= $(DIR_INT)/converters/ft_itoa.c
SRCS_MAN			+= $(DIR_INT)/printers/ft_putnbr_fd.c

SRCS_MAN			+= $(DIR_MEM)/comparators/ft_memcmp.c
SRCS_MAN			+= $(DIR_MEM)/copiers/ft_memcpy.c
SRCS_MAN			+= $(DIR_MEM)/copiers/ft_memmove.c
SRCS_MAN			+= $(DIR_MEM)/initializers/ft_bzero.c
SRCS_MAN			+= $(DIR_MEM)/initializers/ft_calloc.c
SRCS_MAN			+= $(DIR_MEM)/initializers/ft_memset.c
SRCS_MAN			+= $(DIR_MEM)/searchers/ft_memchr.c

SRCS_MAN			+= $(DIR_STR)/comparators/ft_strncmp.c
SRCS_MAN			+= $(DIR_STR)/concatenaters/ft_strlcat.c
SRCS_MAN			+= $(DIR_STR)/converters/ft_atoi.c
SRCS_MAN			+= $(DIR_STR)/converters/ft_striteri.c
SRCS_MAN			+= $(DIR_STR)/copiers/ft_strlcpy.c
SRCS_MAN			+= $(DIR_STR)/initializers/ft_split.c
SRCS_MAN			+= $(DIR_STR)/initializers/ft_strdup.c
SRCS_MAN			+= $(DIR_STR)/initializers/ft_strjoin.c
SRCS_MAN			+= $(DIR_STR)/initializers/ft_strmapi.c
SRCS_MAN			+= $(DIR_STR)/initializers/ft_strtrim.c
SRCS_MAN			+= $(DIR_STR)/initializers/ft_substr.c
SRCS_MAN			+= $(DIR_STR)/misc/ft_strlen.c
SRCS_MAN			+= $(DIR_STR)/printers/ft_putendl_fd.c
SRCS_MAN			+= $(DIR_STR)/printers/ft_putstr_fd.c
SRCS_MAN			+= $(DIR_STR)/searchers/ft_strchr.c
SRCS_MAN			+= $(DIR_STR)/searchers/ft_strnstr.c
SRCS_MAN			+= $(DIR_STR)/searchers/ft_strrchr.c

SRCS_BON			:= $(DIR_LST)/adders/ft_lstadd_back_bonus.c
SRCS_BON			+= $(DIR_LST)/adders/ft_lstadd_front_bonus.c
SRCS_BON			+= $(DIR_LST)/converters/ft_lstiter_bonus.c
SRCS_BON			+= $(DIR_LST)/deleters/ft_lstclear_bonus.c
SRCS_BON			+= $(DIR_LST)/deleters/ft_lstdelone_bonus.c
SRCS_BON			+= $(DIR_LST)/initializers/ft_lstmap_bonus.c
SRCS_BON			+= $(DIR_LST)/initializers/ft_lstnew_bonus.c
SRCS_BON			+= $(DIR_LST)/misc/ft_lstlast_bonus.c
SRCS_BON			+= $(DIR_LST)/misc/ft_lstsize_bonus.c

OBJS_MAN 			:= $(SRCS_MAN:.c=.o)
OBJS_BON 			:= $(SRCS_BON:.c=.o)

AR 					:= ar
ARFLM 				:= -rcs
ARFLB 				:= -rcs

UNAME 				:= $(shell uname -s)
ifeq 				($(UNAME), Linux)
	VG 				:= valgrind
	VGFL 			:= --leak-check=full --show-leak-kinds=all \
						--track-origins=yes
endif

all: 				$(NAME)

$(NAME): 			$(OBJS_MAN)
					make -C $(DIR_PF)
					$(AR) $(ARFLM) $@ $(OBJS_MAN)

bonus: 				$(OBJS_BON)
					$(AR) $(ARFLB) $(NAME) $^

%.o: 				%.c $(LFT_HDR)
					$(CC) $(CCFL) -I$(INCDIR) -c $< -o $@
tests/%: 			all
					@$(CC) $(CCFL) -I$(INCDIR) ./$@.c -l$(LIB) -L$(LIBDIR)
					@./a.out
mtests/%:			tests/%
					$(VG) $(VGFL) ./a.out

fclean: 			clean
					@-rm -f $(NAME)
clean:
					@make -C $(DIR_PF) clean
					@rm -f $(OBJS_MAN)
					@rm -f $(OBJS_BON)
					@rm -f a.out

re: 				fclean all

.PHONY: 			all bonus re fclean clean
