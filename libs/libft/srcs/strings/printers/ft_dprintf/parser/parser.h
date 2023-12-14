/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 14:13:27 by jgermany          #+#    #+#             */
/*   Updated: 2023/06/27 18:17:53 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H

# define PARSER_H

# include <stdint.h>

# include "../commons/ft_dprintf_commons.h"
# include "../printer/printer.h"

int	proc_specif(char *spec, va_list *args, t_meta *meta);

#endif
