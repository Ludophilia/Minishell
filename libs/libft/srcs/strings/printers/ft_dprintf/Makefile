# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/13 14:16:47 by jgermany          #+#    #+#              #
#    Updated: 2023/08/12 13:53:11 by jgermany         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LFTDIR							:= ../../../..
INCDIR							:= ../../../../includes

NAME							:= $(LFTDIR)/libft.a

CC								:= cc
CFLAGS							:= -Wall -Wextra -Werror 
CFLAGS							+= -I$(INCDIR)

AR								:= ar
ARFL							:= -rcs

ROOT							:= .

CMDR							:= $(ROOT)/commons
PADR							:= $(ROOT)/parser
PRDR							:= $(ROOT)/printer

ROOT_HDR						:= $(ROOT)/ft_dprintf.h
MAN_SRCS						:= $(ROOT)/ft_dprintf.c

CM_HDR							:= $(CMDR)/ft_dprintf_commons.h
MAN_SRCS						+= $(CMDR)/ft_dprintf_commons.c

PA_HDR							:= $(PADR)/parser.h
MAN_SRCS						+= $(PADR)/parser.c

PR_HDR							:= $(PRDR)/printer.h
MAN_SRCS						+= $(PRDR)/printer.c

MAN_OBJS						:= $(MAN_SRCS:.c=.o)

all:							$(NAME)

$(NAME):						$(MAN_OBJS)
								$(AR) $(ARFL) $@ $^

%.o:							$(ROOT_HDR)

commons/%.o:					$(CM_HDR)

parser/%.o:						$(PA_HDR)

printer/%.o:					$(PR_HDR)

clean:
								rm -f $(MAN_OBJS)

fclean:							clean
								rm -f $(NAME)

re:
								make fclean
								make all

.DELETE_ON_ERROR:
.PHONY:							all clean fclean re
.SILENT:						clean fclean
