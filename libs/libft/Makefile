# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jegerman <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/06 12:44:08 by jegerman          #+#    #+#              #
#    Updated: 2025/08/31 18:46:44 by jegerman         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := libft.a

CC := cc
AR := ar
CFLAGS := -Wall -Wextra -Werror
ARFLAGS := rcs

SRCS_LFTD := srcs/libft
SRCS_PFBD := srcs/ft_printf
SRCS_GNLBD := srcs/get_next_line

INCL_LFT := includes
INCL_PFB := includes/ft_printf
INCL_GNLB := includes/get_next_line

SRCS_LFT := $(SRCS_LFTD)/ft_isalpha.c
SRCS_LFT += $(SRCS_LFTD)/ft_isdigit.c
SRCS_LFT += $(SRCS_LFTD)/ft_isalnum.c
SRCS_LFT += $(SRCS_LFTD)/ft_isascii.c
SRCS_LFT += $(SRCS_LFTD)/ft_isprint.c
SRCS_LFT += $(SRCS_LFTD)/ft_strlen.c
SRCS_LFT += $(SRCS_LFTD)/ft_memset.c
SRCS_LFT += $(SRCS_LFTD)/ft_bzero.c
SRCS_LFT += $(SRCS_LFTD)/ft_memcpy.c
SRCS_LFT += $(SRCS_LFTD)/ft_memmove.c
SRCS_LFT += $(SRCS_LFTD)/ft_strlcpy.c
SRCS_LFT += $(SRCS_LFTD)/ft_strlcat.c
SRCS_LFT += $(SRCS_LFTD)/ft_toupper.c
SRCS_LFT += $(SRCS_LFTD)/ft_tolower.c
SRCS_LFT += $(SRCS_LFTD)/ft_strchr.c
SRCS_LFT += $(SRCS_LFTD)/ft_strrchr.c
SRCS_LFT += $(SRCS_LFTD)/ft_strncmp.c
SRCS_LFT += $(SRCS_LFTD)/ft_memchr.c
SRCS_LFT += $(SRCS_LFTD)/ft_memcmp.c
SRCS_LFT += $(SRCS_LFTD)/ft_strnstr.c
SRCS_LFT += $(SRCS_LFTD)/ft_atoi.c
SRCS_LFT += $(SRCS_LFTD)/ft_calloc.c
SRCS_LFT += $(SRCS_LFTD)/ft_strdup.c
SRCS_LFT += $(SRCS_LFTD)/ft_substr.c
SRCS_LFT += $(SRCS_LFTD)/ft_strjoin.c
SRCS_LFT += $(SRCS_LFTD)/ft_strtrim.c
SRCS_LFT += $(SRCS_LFTD)/ft_split.c
SRCS_LFT += $(SRCS_LFTD)/ft_itoa.c
SRCS_LFT += $(SRCS_LFTD)/ft_strmapi.c
SRCS_LFT += $(SRCS_LFTD)/ft_striteri.c
SRCS_LFT += $(SRCS_LFTD)/ft_putchar_fd.c
SRCS_LFT += $(SRCS_LFTD)/ft_putstr_fd.c
SRCS_LFT += $(SRCS_LFTD)/ft_putendl_fd.c
SRCS_LFT += $(SRCS_LFTD)/ft_putnbr_fd.c
SRCS_LFT += $(SRCS_LFTD)/ft_lstnew_bonus.c
SRCS_LFT += $(SRCS_LFTD)/ft_lstadd_front_bonus.c
SRCS_LFT += $(SRCS_LFTD)/ft_lstsize_bonus.c
SRCS_LFT += $(SRCS_LFTD)/ft_lstlast_bonus.c
SRCS_LFT += $(SRCS_LFTD)/ft_lstadd_back_bonus.c
SRCS_LFT += $(SRCS_LFTD)/ft_lstdelone_bonus.c
SRCS_LFT += $(SRCS_LFTD)/ft_lstclear_bonus.c
SRCS_LFT += $(SRCS_LFTD)/ft_lstiter_bonus.c
SRCS_LFT += $(SRCS_LFTD)/ft_lstmap_bonus.c

SRCS_PFB := $(SRCS_PFBD)/ft_printf_bonus.c
SRCS_PFB += $(SRCS_PFBD)/printer_bonus.c
SRCS_PFB += $(SRCS_PFBD)/processor_bonus.c
SRCS_PFB += $(SRCS_PFBD)/parser_bonus.c
SRCS_PFB += $(SRCS_PFBD)/parser_helpers_bonus.c
SRCS_PFB += $(SRCS_PFBD)/processor_helpers_bonus.c
SRCS_PFB += $(SRCS_PFBD)/flag_helpers_bonus.c
SRCS_PFB += $(SRCS_PFBD)/bufferer_bonus.c
SRCS_PFB += $(SRCS_PFBD)/bufferer_helpers_bonus.c

SRCS_GNLB := $(SRCS_GNLBD)/get_next_line_bonus.c

OBJ_LFT := $(SRCS_LFT:.c=.o)
OBJ_PFB := $(SRCS_PFB:.c=.o)
OBJ_GNLB := $(SRCS_GNLB:.c=.o)

OBJS := $(OBJ_LFT) $(OBJ_PFB) $(OBJ_GNLB)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(SRCS_LFTD)/%.o: $(SRCS_LFTD)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< -I$(INCL_LFT)

$(SRCS_PFBD)/%.o: $(SRCS_PFBD)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< -I$(INCL_PFB) -I$(INCL_LFT)

$(SRCS_GNLBD)/%.o: $(SRCS_GNLBD)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< -I$(INCL_GNLB) -I$(INCL_LFT)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
