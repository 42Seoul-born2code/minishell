#include "utils.h"

int	print_error(char *msg, char *arg)
{
	char	*error_msg;

	error_msg = ft_strjoin(msg, arg);
	ft_putstr_fd(error_msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	free(error_msg);
	return (EXIT_ERROR);
}

// TODO: 완성 후 삭제하기
void	print_env_list(t_env_list *env_list)
{
	t_list		*list_node;
	t_env_node	*env_node;

	list_node = env_list->head_node;
	printf("==============================================");
	while (list_node != NULL)
	{
		
		env_node = list_node->content;
		printf("%s = %s\n", env_node->key,env_node->value);
		list_node = list_node->next;
	}
}