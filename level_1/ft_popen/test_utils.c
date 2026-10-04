#include <stdlib.h>
#include <unistd.h>

void	ft_putstr(char *s)
{
	int	len;

	len = 0;
	while (s[len])
		len++;
	write(1, s, len);
}

char	*get_next_line(int fd)
{
	char	*line;
	char	*tmp;
	char	c;
	int		len;
	int		cap;
	int		i;

	if (fd < 0)
		return (NULL);
	cap = 64;
	len = 0;
	line = malloc(cap);
	if (!line)
		return (NULL);
	while (read(fd, &c, 1) == 1)
	{
		if (len + 2 > cap)
		{
			cap *= 2;
			tmp = malloc(cap);
			if (!tmp)
				return (free(line), NULL);
			i = -1;
			while (++i < len)
				tmp[i] = line[i];
			free(line);
			line = tmp;
		}
		line[len++] = c;
		if (c == '\n')
			break ;
	}
	if (len == 0)
		return (free(line), NULL);
	line[len] = '\0';
	return (line);
}
