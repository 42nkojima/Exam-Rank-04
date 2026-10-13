#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int	picoshell(char **cmds[])
{
	int		fd[2];
	int		status;
	int		in_fd;
	pid_t	pid;
	int		ret;

	in_fd = -1, ret = 0;
	for (int i = 0; cmds[i]; i++)
	{
		if (cmds[i + 1] && pipe(fd) < 0)
		{
			ret = 1;
			break ;
		}
		if ((pid = fork()) < 0)
		{
			if (cmds[i + 1])
				(close(fd[0]), close(fd[1]));
			ret = 1;
			break ;
		}
		if (pid == 0)
		{
			if (in_fd != -1)
			{
				dup2(in_fd, 0);
				close(in_fd);
			}
			if (cmds[i + 1])
			{
				dup2(fd[1], 1);
				close(fd[0]);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (in_fd != -1)
			close(in_fd);
		in_fd = -1;
		if (cmds[i + 1])
		{
			close(fd[1]);
			in_fd = fd[0];
		}
	}
	if (in_fd != -1)
		close(in_fd);
	while (wait(&status) > 0)
		if (!WIFEXITED(status) || WEXITSTATUS(status))
			ret = 1;
	return (ret);
}

#include <stdio.h>

int	main(void)
{
	char	*cmd1[] = {"/bin/ls", "level-1", NULL};
	char	*cmd2[] = {"/usr/bin/grep", "picoshell", NULL};
	char	**cmds[] = {cmd1, cmd2, NULL};
	int		result;

	write(1, "Test picoshell_short\n", 21);
	// char *cmd1[] = {"/bin/ls", NULL};
	result = picoshell(cmds);
	printf("picoshell returned %d\n", result);
	return (0);
}
