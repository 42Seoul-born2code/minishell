#include "builtin.h"

int	ft_env(char **argv, t_env_list *env_list)
{
	t_list		*list_node;
	t_env_node	*env_node;

	list_node = env_list->head_node;
	if (*(argv + 1))
	{
		print_error(SYNTAX_ERROR, "\n");
		return (EXIT_FAILURE);
	}
	while (list_node)
	{
		env_node = list_node->content;
		if (env_node->value)
			printf("%s=%s\n", env_node->key, env_node->value);
		list_node = list_node->next;
	}
	return (EXIT_SUCCESS);
}
