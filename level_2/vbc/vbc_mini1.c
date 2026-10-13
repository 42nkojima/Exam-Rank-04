#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	unexpected(char c)
{
	if (c)
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of input\n");
	exit(1);
}
int		parse_add(char **s);

int	parse_factor(char **s)
{
	int	v;

	if (isdigit(**s))
	{
		v = **s - '0';
		(*s)++;
		return (v);
	}
	if (**s != '(')
		unexpected(**s);
	(*s)++;
	v = parse_add(s);
	if (**s != ')')
		unexpected(**s);
	(*s)++;
	return (v);
}

int	parse_mul(char **s)
{
	int	v;

	v = parse_factor(s);
	while (**s == '*')
	{
		(*s)++;
		v *= parse_factor(s);
	}
	return (v);
}
int	parse_add(char **s)
{
	int	v;

	v = parse_mul(s);
	while (**s == '+')
	{
		(*s)++;
		v += parse_mul(s);
	}
	return (v);
}
int	main(int argc, char **argv)
{
	int	v;

	if (argc != 2)
		return (1);
	v = parse_add(&argv[1]);
	if (*argv[1])
		unexpected(*argv[1]);
	printf("%d\n", v);
	return (0);
}
