#include "builtin.h"

int ft_env(char **argv, t_env_list *env_list)
{
	t_list		*list_node;
	t_env_node 	*env_node;

	list_node = env_list->head_node;
	if (*(argv + 1))
	{
		printf("NO OPTION\n");
		return (EXIT_FAILURE);
	}
	while (list_node)
	{
		env_node = list_node->content;
		if (env_node->value)
		{
			ft_putstr_fd(env_node->key, STDOUT_FILENO);
			write(STDOUT_FILENO, "=", 1);
			ft_putstr_fd(env_node->value, STDOUT_FILENO);
			write(STDOUT_FILENO, "\n", 1);
		}
		list_node = list_node->next;
	}
	return (EXIT_SUCCESS);
}