/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:58:50 by jegerman          #+#    #+#             */
/*   Updated: 2025/04/23 20:46:26 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_BONUS_H

# define FT_PRINTF_BONUS_H

# define BASE10 "0123456789"
# define BASE16_LW "0123456789abcdef"
# define BASE16_UP "0123456789ABCDEF"

# define ZEROFILL 1
# define SPACEFILL 0

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>

# include "../libft/libft_bonus.h"

typedef enum e_flg
{
	FLG_DASH = (1 << 0),
	FLG_ZERO = (1 << 1),
	FLG_PREC = (1 << 2),
	FLG_FIEL = (1 << 3),
	FLG_POUN = (1 << 4),
	FLG_SPAC = (1 << 5),
	FLG_PLUS = (1 << 6)
}	t_flg;

typedef enum e_type
{
	CV_INT = (1 << 7),
	CV_HEX = (1 << 8),
	CV_HEXL = (1 << 9),
	CV_HEXU = (1 << 10),
	CV_UINT = (1 << 11),
	CV_PTR = (1 << 12),
	CV_STR = (1 << 13),
	CV_CHR = (1 << 14),
	CV_PRC = (1 << 15),
	CV_NBR = CV_INT | CV_UINT | CV_HEX
}	t_type;

typedef struct s_nbr
{
	char			*base;
	int				sign;
	unsigned long	magn;
	int				magn_len;
	char			*pad_magn;
}	t_nbr;

typedef union u_usl
{
	unsigned long	u;
	long			s;
}	t_usl;

typedef struct s_meta
{
	unsigned int	flags;
	int				fd;
	int				field_v;
	int				prec_v;
	va_list			args;
	int				*i;
	int				count;
}	t_meta;

int		buffer_nbr_base(t_nbr *nbr, char *buffer, int *i, t_meta *m);
char	*set_magnitude_buffer(t_nbr *nbr, t_meta *m);

int		ft_putchar_cc(const char c, t_meta *m);
int		ft_putstr_cc(const char *str, t_meta *m);
void	print_filler(bool zfill, t_meta *m);
int		print_prefix(t_nbr *nbr, t_meta *m);

int		flags(unsigned int flags, t_meta *m);
int		flag(unsigned int flags, t_meta *m);
int		not_flags(unsigned int flags, t_meta *m);
int		not_flag(unsigned int flags, t_meta *m);

void	process_filler_width(void *data, t_meta *m);
void	process_specifier(t_meta *m);

int		set_conv(char c, t_meta *m);
bool	is_valid_flg(char c);
bool	is_valid_conv(char c);

int		is_valid_specif(const char *spec, t_meta *m);

int		ft_printf(const char *format, ...);
int		ft_dprintf(int fd, const char *format, ...);
int		ft_eprintf(const char *format, ...);

#endif
