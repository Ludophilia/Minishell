/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:05:44 by jegerman          #+#    #+#             */
/*   Updated: 2024/12/21 17:58:45 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

static int	pre_proc_flags(const char *spec, int limit, t_meta *m)
{
	int	j;

	j = -1;
	while (spec[++j] && j < limit)
	{
		if (spec[j] == '-' && not_flag(CV_PRC, m))
			m->flags |= FLG_DASH;
		else if (spec[j] == '.' && flag(CV_STR | CV_NBR, m)
			&& (is_valid_conv(spec[j + 1]) || ft_isdigit(spec[j + 1])))
			m->flags |= FLG_PREC;
	}
	return (1);
}

static int	pre_proc_specif(const char *spec, t_meta *m)
{
	int	i;

	i = 0;
	while (spec[i] && is_valid_conv(spec[i]) == 0)
		i++;
	if (spec[i] && is_valid_conv(spec[i]) && set_conv(spec[i], m))
	{
		pre_proc_flags(spec, i, m);
		return (1);
	}
	m->flags = 0;
	return (0);
}

static int	proc_flags(const char *spec, t_meta *m)
{
	int	i;

	i = 0;
	while (spec[i] && is_valid_flg(spec[i]))
	{
		if (spec[i] == ' ' && flag(CV_INT, m))
			m->flags |= FLG_SPAC;
		else if (spec[i] == '+' && flag(CV_INT, m))
			m->flags |= FLG_PLUS;
		else if (spec[i] == '#' && flag(CV_HEX, m))
			m->flags |= FLG_POUN;
		else if (spec[i] == '0' && not_flag(FLG_DASH | FLG_PREC, m)
			&& flag(CV_NBR, m))
			m->flags |= FLG_ZERO;
		i++;
	}
	return (i);
}

static int	proc_width_options(const char *spec, t_meta *m)
{
	int	i;

	i = 0;
	if (spec[i] && ft_isdigit(spec[i]) && not_flag(CV_PRC, m))
	{
		m->flags |= FLG_FIEL;
		m->field_v = ft_atoi(spec);
	}
	while (spec[i] && ft_isdigit(spec[i]))
		i++;
	if (spec[i] == '.')
		i++;
	if (spec[i] && flag(FLG_PREC, m) && is_valid_conv(spec[i]))
		m->prec_v = 0;
	else if (spec[i] && flag(FLG_PREC, m) && ft_isdigit(spec[i]))
		m->prec_v = ft_atoi(spec + i);
	while (spec[i] && ft_isdigit(spec[i]))
		i++;
	return (i);
}

int	is_valid_specif(const char *spec, t_meta *m)
{
	int	i;

	m->flags = 0;
	if (pre_proc_specif(spec, m) == 0)
		return (0);
	m->field_v = -1;
	m->prec_v = -1;
	i = 0;
	i += proc_flags(spec + i, m);
	i += proc_width_options(spec + i, m);
	if (spec[i] && is_valid_conv(spec[i]))
	{
		*m->i += (i);
		return (1);
	}
	return (0);
}
