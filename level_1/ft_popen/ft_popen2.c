// #include <stdlib.h>
// #include <sys/types.h>
// #include <unistd.h>

// int	ft_popen(const char *file, char *const argv[], char type)
// {
// 	int		fd[2];
// 	pid_t	pid;

// 	if (!file || !argv || (type != 'r' && type != 'w') || pipe(fd) == -1)
// 		return (-1);
// 	if ((pid = fork()) == -1)
// 	{
// 		close(fd[0]);
// 		close(fd[1]);
// 		return (-1);
// 	}
// 	if (pid == 0)
// 	{
// 		if (dup2(fd[type == 'r' ? 1 : 0], type == 'r' ? 1 : 0) == -1)
// 			exit(1);
// 		close(fd[0]);
// 		close(fd[1]);
// 		execvp(file, argv);
// 		exit(1);
// 	}
// 	close(fd[type == 'r' ? 1 : 0]);
// 	return (type == 'r' ? 0 : 1);
// }
