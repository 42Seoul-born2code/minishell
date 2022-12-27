#include "minishell.h"

/*
	get_envp_in_list()				- Convert data type t_env_list into char **
	
	return							- Current envp(char **)
	curr_env						- Current env list (t_env_list)
	list_size						- Current env list size
	envp							- Result converting from t_env_list to char **
	curr_node						- Current node in iterating linkedlist
*/
char	**get_envp_in_list(t_env_list *curr_env)
{
	int			idx;
	int			list_size;
	char		**envp;
	t_list		*list_node;
	t_env_node	*curr_node;

	idx = 0;
	list_node = curr_env->head_node;
	list_size = ft_lstsize(list_node);
	envp = malloc(sizeof(char *) * (list_size + 1));
	while (list_node != NULL)
	{
		curr_node = list_node->content;
		envp[idx] = ft_strjoin(curr_node->key, curr_node->value);
		idx += 1;
		list_node = list_node->next;
	}
	return (envp);
}

/*
	save_envp_in_list()				- Save envp in 3 ways line, key, value by linked list
	
	return							- void
	envp							- main function envp
	curr_env						- Current env list (t_env_list)
	i								- index of envp
	kv_pair							- split by token '=' envp 
	env_node						- temp node that exit for adding curr_env 
*/
void	save_envp_in_list(char **envp, t_env_list *curr_env)
{
	int			i;
	char		**kv_pair;
	t_env_node	*env_node;

	i = 0;
	curr_env->head_node = NULL;
	while (envp[i] != NULL)
	{
		kv_pair = ft_split(envp[i], '=');
		env_node = malloc(sizeof(t_env_node));
		env_node->key = kv_pair[0];
		env_node->value = kv_pair[1];
		ft_lstadd_back(&curr_env->head_node, ft_lstnew(env_node));
		i += 1;
	}
}
