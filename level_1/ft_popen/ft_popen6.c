#include <stdlib.h>
#include <unistd.h>

int	ft_popen(const char *file, char *const argv[], char type)
{
	int	fd[2];
	int	c;

	if ((type != 'r' && type != 'w') || pipe(fd) == -1)
		return (-1);
	c = (type == 'r');
	if (fork() == 0)
	{
		dup2(fd[c], c);
		close(fd[0]);
		close(fd[1]);
		execvp(file, argv);
		exit(1);
	}
	close(fd[c]);
	return (fd[!c]);
}
