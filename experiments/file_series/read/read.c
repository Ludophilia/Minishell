#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 10

int	cli_init(int *fd, char **buffer, int argc, char **argv)
{
	if (argc != 2)
	{
		dprintf(2, "Wrong number of arguments\n"
				"usage: test <path>\n");
		return (-1);
	}
	*buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (*buffer == NULL)
		return (-1);
	*fd = open(argv[1], O_RDONLY);
	if (*fd == -1 || memset(*buffer, 0, BUFFER_SIZE + 1) == NULL)
	{
		free(*buffer);
		perror("Something wrong happened.");
		return (-1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	int		fd;
	ssize_t	bytesread;
	char	*buffer;

	if (cli_init(&fd, &buffer, argc, argv) == -1)
		return (1);
	bytesread = read(fd, buffer, BUFFER_SIZE);
	while (bytesread)
	{
		if (bytesread == -1)
		{
			perror("Something wrong happened.");
			close(fd);
			free(buffer);
			return (2);
		}
		printf("%s", buffer);
		bytesread = read(fd, buffer, BUFFER_SIZE);
	}
	close(fd);
	free(buffer);
	return (0);
}
