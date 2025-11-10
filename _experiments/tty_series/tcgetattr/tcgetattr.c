/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcgetattr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 17:25:36 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/10 19:05:44 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <stdio.h>

void	main_break_down_input_flags(int proceed, struct termios *termios)
{
	if (proceed == 0)
		return ;
	printf("input mode flags: %#x\n", termios->c_iflag);
	printf("\tignore break condition: %#x\n", termios->c_iflag & IGNBRK);
	printf("\ta BREAK causes the input and output queues to be flushed: %#x\n",
		termios->c_iflag & BRKINT);
	printf("\tignore framing errors and parity errors: %#x\n",
		termios->c_iflag & IGNPAR);
	printf("\tenable input parity checking: %#x\n", termios->c_iflag & INPCK);
	printf("\tstrip off eighth bit: %#x\n", termios->c_iflag & ISTRIP);
	printf("\ttranslate NL to CR on input: %#x\n",
		termios->c_iflag & INLCR);
	printf("\tignore carriage return on input: %#x\n",
		termios->c_iflag & IGNCR);
	printf("\ttranslate carriage return to newline on input: %#x\n",
		termios->c_iflag & ICRNL);
	printf("\tmap uppercase characters to lowercase on input: %#x\n",
		termios->c_iflag & IUCLC);
	printf("\tenable XON (XOFF) flow control on output.: %#x\n",
		termios->c_iflag & IXON);
	printf("\ttyping any character will restart stopped output.: %#x\n",
		termios->c_iflag & IXANY);
	printf("\tenable XOFF (XON) flow control on input: %#x\n",
		termios->c_iflag & IXOFF);
	printf("\tring bell when input queue is full: %#x\n",
		termios->c_iflag & IMAXBEL);
	printf("\tinput is UTF8: %#x\n", termios->c_iflag & IUTF8);
}

void	main_break_down_output_flags(int proceed, struct termios *termios)
{
	if (proceed == 0)
		return ;
	printf("output mode flags: %#x\n", termios->c_oflag);
	printf("\tenable implementation-defined output processing: %#x\n",
		termios->c_oflag & OPOST);
	printf("\tmap lowercase characters to uppercase on output: %#x\n",
		termios->c_oflag & OLCUC);
	printf("\tmap NL to CR-NL on output: %#x\n",
		termios->c_oflag & ONLCR);
	printf("\tdon't output CR at column 0: %#x\n",
		termios->c_oflag & ONOCR);
	printf("\tthe NL character assumed to do carriage-return function: %#x\n",
		termios->c_oflag & ONLRET);
	printf("\tsend fill characters for a delay rather than using"
		" a timed delay: %#x\n", termios->c_oflag & OFILL);
	printf("\tfill character is ASCII DEL (0177): %#x\n",
		termios->c_oflag & OFDEL);
	printf("\t(more on the man....)\n");
}

void	main_break_down_control_flags(int proceed, struct termios *termios)
{
	if (proceed == 0)
		return ;
	printf("control mode flags: %#x\n", termios->c_cflag);
	printf("\tcharacter size mask: %#x\n", termios->c_cflag & CSIZE);
	printf("\tset two stop bits, rather than one: %#x\n",
		termios->c_cflag & CSTOPB);
	printf("\tenable receiver: %#x\n", termios->c_cflag & CREAD);
	printf("\tenable parity generation on output "
		"and parity checking for input: %#x\n", termios->c_cflag & PARENB);
	printf("\tparity for input and output is odd;"
		" otherwise even parity is used: %#x\n", termios->c_cflag & PARODD);
	printf("\tlower modem control lines after last process closes the device"
		": %#x\n", termios->c_cflag & HUPCL);
	printf("\tignore modem control lines."
		": %#x\n", termios->c_cflag & CLOCAL);
	printf("\t(more on the man....)\n");
}

void	main_break_down_local_flags(int proceed, struct termios *termios)
{
	if (proceed == 0)
		return ;
	printf("local mode flags: %#x\n", termios->c_lflag);
	printf("\t(more on the man....)\n");
}

void	main_break_down_special_chars(int proceed, struct termios *termios)
{
	int	i;

	if (proceed == 0)
		return ;
	i = -1;
	while (++i < NCCS)
		printf("special characters: %i\n", termios->c_cc[i]);
}

int	main_get_term_attrs(struct termios *termios)
{
	if (tcgetattr(0, termios) == -1)
		return (-1);
	main_break_down_input_flags(0, termios);
	main_break_down_output_flags(0, termios);
	main_break_down_control_flags(1, termios);
	main_break_down_local_flags(0, termios);
	main_break_down_special_chars(0, termios);
	return (0);
}

int	main(void)
{
	struct termios	termios;

	if (main_get_term_attrs(&termios) == -1)
		return (1);
	return (0);
}
