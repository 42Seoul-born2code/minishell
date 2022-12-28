#include "utils.h"

/*
	get_envp_in_list()				- Convert data type t_env_list into char **
	
	return							- Current envp(char **)
	env_list						- Current env list (t_env_list)
	list_size						- Current env list size
	envp							- Result converting from t_env_list to char **
	curr_node						- Current node in iterating linkedlist
*/
char	**get_envp_in_list(t_env_list *env_list)
{
	int			idx;
	int			list_size;
	char		*buffer;
	char		**envp;
	t_list		*list_node;

	idx = 0;
	list_node = env_list->head_node;
	list_size = ft_lstsize(list_node);
	buffer = NULL;
	envp = malloc(sizeof(char *) * (list_size + 1));
	while (list_node != NULL)
	{
		buffer = ft_strjoin(((t_env_node *)list_node->content)->key, "=");
		envp[idx] = ft_strjoin(buffer, \
								((t_env_node *)list_node->content)->value);
		free(buffer);
		idx += 1;
		list_node = list_node->next;
	}
	envp[idx] = NULL;
	return (envp);
}

/*
	save_envp_in_env_list()				- Save envp in 3 ways line, key, value by linked list
	
	return							- void
	envp							- main function envp
	env_list						- Current env list (t_env_list)
	i								- index of envp
	kv_pair							- split by token '=' envp 
	env_node						- temp node that exit for adding env_list 
*/
void	save_envp_in_env_list(char **envp, t_env_list *env_list)
{
	int			idx;
	char		**kv_pair;
	t_env_node	*env_node;

	idx = 0;
	env_list->head_node = NULL;
	while (envp[idx] != NULL)
	{
		kv_pair = ft_split(envp[idx], '=');
		env_node = malloc(sizeof(t_env_node));
		env_node->key = kv_pair[0];
		env_node->value = kv_pair[1];
		ft_lstadd_back(&env_list->head_node, ft_lstnew(env_node));
		idx += 1;
	}
}

t_env_list	*init_env_list(char **envp)
{
	t_env_list	*env_list;

	env_list = malloc(sizeof(t_env_list));
	env_list->head_node = NULL;
	save_envp_in_env_list(envp, env_list);
	return (env_list);
}
