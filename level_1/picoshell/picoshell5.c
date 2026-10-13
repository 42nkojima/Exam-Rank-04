#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int	picoshell(char **cmds[])
{
	int	fd[2];
	int	in;
	int	ret;

	in = 0;
	ret = 0;
	for (int i = 0; cmds[i]; i++)
	{
		if (cmds[i + 1] && pipe(fd) < 0)
			ret = 1;
		if (ret)
			break ;
		if (fork() == 0)
		{
			if (in)
				(dup2(in, 0), close(in));
			if (cmds[i + 1])
				(dup2(fd[1], 1), close(fd[0]), close(fd[1]));
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (in)
			close(in);
		in = 0;
		if (cmds[i + 1])
			(close(fd[1]), in = fd[0]);
	}
	if (in)
		close(in);
	while (wait(NULL) > 0)
		;
	return (ret);
}
