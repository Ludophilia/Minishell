# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jegerman <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/09 12:08:14 by jegerman          #+#    #+#              #
#    Updated: 2025/10/17 20:08:48 by jegerman         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

CC := cc
CFLAGS := -Wall -Wextra -Werror -g3

LIBS_DR := libs
LIBFT_DR := $(LIBS_DR)/libft
SRCS_DR := srcs
ICL_DR := includes

LFT := $(LIBFT_DR)/libft.a

CFLAGS += -I$(ICL_DR) -I$(LIBFT_DR)/includes
LIBCFLAGS := -L$(LIBFT_DR) -lft -lreadline

CORE_DR := $(SRCS_DR)/core
BLTN_DR := $(SRCS_DR)/builtins
SGNL_DR := $(SRCS_DR)/signals
LEXR_DR := $(SRCS_DR)/lexer
PRSR_DR := $(SRCS_DR)/parser
FMGR_DR := $(SRCS_DR)/fmgr
EXEC_DR := $(SRCS_DR)/exec
ENVM_DR := $(SRCS_DR)/envmgr

MAN_SRCS := $(CORE_DR)/main.c
MAN_SRCS += $(CORE_DR)/loop.c
MAN_SRCS += $(CORE_DR)/init.c
MAN_SRCS += $(CORE_DR)/utils.c

MAN_SRCS += $(LEXR_DR)/lexer.c
MAN_SRCS += $(LEXR_DR)/lexer_utils.c

MAN_SRCS += $(PRSR_DR)/parser.c
MAN_SRCS += $(PRSR_DR)/parser_cmds.c
MAN_SRCS += $(PRSR_DR)/parser_error.c
MAN_SRCS += $(PRSR_DR)/parser_reds.c
MAN_SRCS += $(PRSR_DR)/parser_word.c
MAN_SRCS += $(PRSR_DR)/parser_envv.c
MAN_SRCS += $(PRSR_DR)/parser_utils.c
MAN_SRCS += $(PRSR_DR)/parser_cleanup.c

MAN_SRCS += $(FMGR_DR)/fmgr.c
MAN_SRCS += $(FMGR_DR)/fmgr_reds.c
MAN_SRCS += $(FMGR_DR)/fmgr_utils.c

MAN_SRCS += $(EXEC_DR)/exec.c
MAN_SRCS += $(EXEC_DR)/exec_path.c
MAN_SRCS += $(EXEC_DR)/exec_builtins.c

MAN_SRCS += $(BLTN_DR)/cd.c
MAN_SRCS += $(BLTN_DR)/echo.c
MAN_SRCS += $(BLTN_DR)/env.c
MAN_SRCS += $(BLTN_DR)/exit.c
MAN_SRCS += $(BLTN_DR)/export.c
MAN_SRCS += $(BLTN_DR)/pwd.c
MAN_SRCS += $(BLTN_DR)/unset.c

MAN_SRCS += $(ENVM_DR)/env_builder.c
MAN_SRCS += $(ENVM_DR)/env_utils.c
MAN_SRCS += $(ENVM_DR)/env_formatter.c
MAN_SRCS += $(ENVM_DR)/env_cleanup.c

MAN_SRCS += $(SGNL_DR)/signals.c

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
