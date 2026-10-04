#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int	ft_popen(const char *file, char *const av[], char type)
{
	int		fd[2];
	pid_t	pid;

	if (!file || !av || (type != 'r' && type != 'w') || pipe(fd) == -1)
		return (-1);
	if ((pid = fork()) == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		if (dup2(fd[type == 'r' ? 1 : 0], type == 'r' ? 1 : 0) == -1)
			exit(1);
		close(fd[0]);
		close(fd[1]);
		execvp(file, av);
		exit(1);
	}
	close(fd[type == 'r' ? 1 : 0]);
	return (fd[type == 'r' ? 0 : 1]);
}

char	*get_next_line(int fd);

int	main(void)
{
	int		fd;
	char	*line;

	fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
	dup2(fd, 0);
	fd = ft_popen("grep", (char *const[]){"grep", "c", NULL}, 'r');
	while ((line = get_next_line(fd)))
		printf("%s", line);
}
