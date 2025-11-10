/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bufferer_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 14:33:43 by jegerman          #+#    #+#             */
/*   Updated: 2024/12/20 16:47:11 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

static void	set_number(t_nbr *nbr, t_meta *m)
{
	t_usl	tmp;

	if (flag(CV_PTR, m))
		tmp.u = va_arg(m->args, unsigned long);
	else if (flag(CV_INT, m))
		tmp.s = va_arg(m->args, int);
	else if (flag(CV_UINT | CV_HEX, m))
		tmp.u = va_arg(m->args, unsigned int);
	if (flag(CV_PTR | CV_HEXL, m))
		*nbr = (t_nbr){.sign = 0, .magn = tmp.u, .base = BASE16_LW};
	else if (flag(CV_HEXU, m))
		*nbr = (t_nbr){.sign = 0, .magn = tmp.u, .base = BASE16_UP};
	else if (flag(CV_UINT, m))
		*nbr = (t_nbr){.sign = 0, .magn = tmp.u, .base = BASE10};
	else if (flag(CV_INT, m) && tmp.s < 0)
		*nbr = (t_nbr){.sign = 1, .magn = -tmp.s, .base = BASE10};
	else if (flag(CV_INT, m) && tmp.s >= 0)
		*nbr = (t_nbr){.sign = 0, .magn = tmp.s, .base = BASE10};
}

static int	get_magnitude_len(t_nbr *nbr, t_meta *m)
{
	int				magn_len;
	int				radix;
	unsigned long	magn;

	magn_len = 0;
	if (nbr->magn == 0 && not_flag(CV_PTR, m))
		magn_len = 1;
	else if (nbr->magn == 0 && flag(CV_PTR, m))
		magn_len = 5;
	radix = ft_strlen(nbr->base);
	magn = nbr->magn;
	while (magn && ++magn_len)
		magn /= radix;
	nbr->magn_len = magn_len;
	if (flag(FLG_PREC, m) && m->prec_v > magn_len)
		magn_len += (m->prec_v - magn_len);
	return (magn_len);
}

char	*set_magnitude_buffer(t_nbr *nbr, t_meta *m)
{
	char	*buffer;
	int		i;
	int		magn_len;
	int		prec_v;

	set_number(nbr, m);
	magn_len = get_magnitude_len(nbr, m);
	buffer = ft_calloc(magn_len + 1, sizeof(char));
	if (buffer == NULL)
		return (NULL);
	i = 0;
	if (flag(FLG_PREC, m) && m->prec_v > 0 && m->prec_v > nbr->magn_len)
	{
		prec_v = m->prec_v;
		prec_v -= nbr->magn_len;
		while (prec_v-- > 0)
			buffer[i++] = '0';
	}
	if (not_flag(FLG_PREC, m) || (flag(FLG_PREC, m)
			&& (nbr->magn != 0 || (nbr->magn == 0 && m->prec_v > 0))))
		buffer_nbr_base(nbr, buffer, &i, m);
	nbr->pad_magn = buffer;
	return (buffer);
}
