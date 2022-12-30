#include "builtin.h"
#include "execute.h"

static int	check_lld_overflow(unsigned long long res, int sign)
{
	if (res > 9223372036854775807UL && sign > 0)
		return (FALSE);
	if (res > 9223372036854775808UL && sign < 0)
		return (FALSE);
	return (TRUE);
}

static int	valid_exit_code(const char *s)
{
	char				*ptr;
	unsigned long long	res;
	int					sign;

	ptr = (char *)s;
	res = 0;
	sign = 1;
	if (*ptr == '-')
	{
		sign = -1;
		ptr++;
	}
	while (*ptr)
	{

		res = res * 10 + (*ptr - '0');
		ptr++;
	}
	return (check_lld_overflow(res, sign));
}

static unsigned char	check_arg(char *arg)
{
	char	*ptr;

	ptr = arg;
	if (*ptr == '-')
		ptr++;
	while (*ptr)
	{
		if (!ft_isdigit(*ptr))
		{
			printf("numeric argument required\n");
			exit(255);
		}
		ptr++;
	}
	if (valid_exit_code(arg) == FALSE)
	{
		printf("numeric argument required\n");
		exit(255);
	}
	return (ft_atoi(arg));
}

static void	free_env_list(t_env_list *env_list)
{
	t_list			*curr_list;
	t_list			*next_list;
	t_env_node		*curr_node;

	curr_list = env_list->head_node;
	while (curr_list != NULL)
	{
		next_list = curr_list->next;
		curr_node = curr_list->content;
		free(curr_node->key);
		if (curr_node->value != NULL) 
		{
			free(curr_node->value);
		}
		free(curr_list);
		curr_list = next_list;
	}
	env_list->head_node = NULL;
}

void	ft_exit(char **argv, t_env_list *env_list)
{
	unsigned char	exit_code;

	exit_code = EXIT_SUCCESS;
	printf("exit\n");
	if (*(argv + 1))
	{
		exit_code = check_arg(*(argv + 1));
		if (*(argv + 2))
			printf("too many arguments\n");
	}
	free_env_list(env_list);
	exit(exit_code);
}
