/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processor_helpers_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:40:37 by jegerman          #+#    #+#             */
/*   Updated: 2024/12/21 16:14:38 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

static void	process_chr_width(t_meta *m)
{
	m->field_v -= 1;
}

static void	process_str_width(char *str, t_meta *m)
{
	int	i;

	i = -1;
	while (str[++i] && m->field_v > 0
		&& ((flag(FLG_PREC, m) && i < m->prec_v) || not_flag(FLG_PREC, m)))
		m->field_v -= 1;
}

static void	process_nbr_width(t_nbr *nbr, t_meta *m)
{
	m->field_v -= ft_strlen(nbr->pad_magn);
	if (flag(CV_INT, m) && (nbr->sign || flag(FLG_PLUS | FLG_SPAC, m)))
		m->field_v -= 1;
	if ((flag(CV_PTR, m) || flags(CV_HEX | FLG_POUN, m)) && nbr->magn != 0)
		m->field_v -= 2;
}

void	process_filler_width(void *data, t_meta *m)
{
	if (flag(CV_STR, m) && data != NULL)
		process_str_width(data, m);
	else if (flag(CV_CHR, m) && not_flag(CV_PRC, m))
		process_chr_width(m);
	else if (flag(CV_NBR | CV_PTR, m) && data != NULL)
		process_nbr_width(data, m);
}
