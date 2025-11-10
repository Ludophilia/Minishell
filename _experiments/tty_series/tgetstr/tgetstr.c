/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tgetstr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgermany <nyaritakunai@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:30:28 by jgermany          #+#    #+#             */
/*   Updated: 2023/12/11 16:42:19 by jgermany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <term.h>

int main()
{
	char		term_buffer[1024];
	const char	*term_type;
	int 		result;

	term_type = "xterm"; // Replace with desired terminal type
	// // Retrieve the entry for the specified terminal type
	result = tgetent(term_buffer, term_type);
	if (result != 1)
	{
		// Error handling if entry retrieval fails
		perror("");
		return (1);
	}

    // Get the string capability value for a specific capability name
    char *clear_screen_plus = tgetstr("clear", NULL);
    if (clear_screen_plus != NULL) {
        // If the capability "clear" is available for this terminal,
        // clear the screen using the retrieved control sequence
        printf("%s", clear_screen_plus);
    } else {
        // Error handling if retrieving the clear-screen sequence fails
        printf("Clear screen sequence not available for this terminal.\n");
    }

    // Get the control sequences for changing text attributes
    char *move_cursor = tgetstr("cm", NULL);
    if (move_cursor != NULL) {
        // If the capability "cm" (cursor movement) is available,
        // move the cursor to position (5, 10)
        printf("%s", tparm(move_cursor, 5, 10));
    } else {
        // Error handling if retrieving cursor movement sequence fails
        printf("Cursor movement sequence not available for this terminal.\n");
    }
	return (0);
}
