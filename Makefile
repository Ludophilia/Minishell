# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/11 14:49:07 by ggay              #+#    #+#              #
#    Updated: 2023/12/13 20:25:53 by jgermany         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

CC := cc
CFLAGS := -Wall -Wextra -Werror

LIBS_DR := libs
LIBFT_DR := $(LIBS_DR)/libft
SRCS_DR := srcs
ICL_DR := includes

LFT := $(LIBFT_DR)/libft.a

CFLAGS += -I$(ICL_DR) -I$(LIBFT_DR)/includes
LIBCFLAGS := -L$(LIBFT_DR) -lft -lreadline

MAN_SRCS := $(SRCS_DR)/main.c
# MAN_SRCS += $(SRCS_DR)/parser_core.c
# MAN_SRCS += $(SRCS_DR)/parser_utils.c
# MAN_SRCS += $(SRCS_DR)/ressources.c

# MAN_SRCS += $(SRCS_DR)/pipex_cmdmgr.c
# MAN_SRCS += $(SRCS_DR)/pipex_filemgr.c
# MAN_SRCS += $(SRCS_DR)/pipex_sanicheck.c

MAN_OBJS := $(MAN_SRCS:.c=.o)

all: $(NAME)

$(LFT):
	make -C $(LIBFT_DR)

$(NAME): $(MAN_OBJS) | $(LFT)
	$(CC) $(CFLAGS) $(MAN_OBJS) -o $@ $(LIBCFLAGS)

$(SRCS_DR)/%.o: $(ICL_DR)/%.h

clean:
	make -C $(LIBFT_DR) clean
	rm -f $(MAN_OBJS)

fclean: clean
	make -C $(LIBFT_DR) fclean
	rm -f $(NAME)

re:
	make fclean
	make all

.PHONY: all clean fclean re
.SILENT: clean fclean
