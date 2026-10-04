#include <ctype.h>
#include <malloc.h>
#include <stdio.h>

typedef struct node
{
	enum
	{
		ADD,
		MULTI,
		VAL
	} type;
	int			val;
	struct node	*l;
	struct node	*r;
}				node;

node	*new_node(node n)
{
	node	*ret;

	ret = calloc(1, sizeof(node));
	if (!ret)
		return (NULL);
	*ret = n;
	return (ret);
}

void	destroy_tree(node *n)
{
	if (!n)
		return ;
	if (n->type != VAL)
	{
		destroy_tree(n->l);
		destroy_tree(n->r);
	}
	free(n);
}

void	unexpected(char c)
{
	if (c)
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of input\n");
}

int	accept(char **s, char c)
{
	if (**s == c)
	{
		(*s)++;
		return (1);
	}
	return (0);
}

int	expect(char **s, char c)
{
	if (accept(s, c))
		return (1);
	unexpected(**s);
	return (0);
}

node			*parse_add(char **s);

node	*parse_factor(char **s)
{
	node	*ret;

	if (isdigit(**s))
	{
		ret = new_node((node){.type = VAL, .val = **s - '0'});
		(*s)++;
		return (ret);
	}
	if (accept(s, '('))
	{
		ret = parse_add(s);
		if (!ret)
			return (NULL);
		if (!expect(s, ')'))
		{
			destroy_tree(ret);
			return (NULL);
		}
		return (ret);
	}
	unexpected(**s);
	return (NULL);
}

node	*parse_mul(char **s)
{
	node	*left;
	node	*right;

	left = parse_factor(s);
	if (!left)
		return (NULL);
	while (accept(s, '*'))
	{
		right = parse_factor(s);
		if (!right)
		{
			destroy_tree(left);
			return (NULL);
		}
		left = new_node((node){.type = MULTI, .l = left, .r = right});
	}
	return (left);
}

node	*parse_add(char **s)
{
	node	*left;
	node	*right;

	left = parse_mul(s);
	if (!left)
		return (NULL);
	while (accept(s, '+'))
	{
		right = parse_mul(s);
		if (!right)
		{
			destroy_tree(left);
			return (NULL);
		}
		left = new_node((node){.type = ADD, .l = left, .r = right});
	}
	return (left);
}

node	*parse_expr(char *s)
{
	node	*ret;

	ret = parse_add(&s);
	if (!ret)
		return (NULL);
	if (*s)
	{
		unexpected(*s);
		destroy_tree(ret);
		return (NULL);
	}
	return (ret);
}

int	eval_tree(node *tree)
{
	switch (tree->type)
	{
	case ADD:
		return (eval_tree(tree->l) + eval_tree(tree->r));
	case MULTI:
		return (eval_tree(tree->l) * eval_tree(tree->r));
	case VAL:
		return (tree->val);
	}
}

int	main(int argc, char **argv)
{
	node	*tree;

	if (argc != 2)
		return (1);
	tree = parse_expr(argv[1]);
	if (!tree)
		return (1);
	printf("%d\n", eval_tree(tree));
	destroy_tree(tree);
}
