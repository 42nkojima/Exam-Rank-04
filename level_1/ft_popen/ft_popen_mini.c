#include <stdlib.h>
#include <unistd.h>

int	ft_poepn(const char *file, char *const av[], char type)
{
	int	fd[2];
	int	c;

	if ((type != 'r' && type != 'w') || pipe(fd) == -1)
		return (-1);
	c = (type == 'r'); // 'r' なら fd[1]->1, 'w' なら fd[0]->0
	if (fork() == 0)
	{
		dup2(fd[c], c);
		close(fd[0]);
		close(fd[1]);
		execvp(file, av);
		exit(1);
	}
	close(fd[c]);
	return (fd[!c]);
}
