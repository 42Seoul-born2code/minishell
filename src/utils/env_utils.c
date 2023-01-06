/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:17:37 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:17:53 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
	is_env_existed()	- Check if environment variable is existed
	
	return				- TRUE or FALSE

	env_list			- Current env list (t_env_list)
	key					- Environment variable name

	list_node			- Node for t_env_list in iterating linked list
	env_node			- Actual node for t_list content
*/
t_bool	is_env_existed(t_env_list *env_list, char *key)
{
	t_list		*list_node;
	t_env_node	*env_node;

	list_node = env_list->head_node;
	while (list_node != NULL)
	{
		env_node = list_node->content;
		if (ft_strcmp(env_node->key, key) == 0)
			return (TRUE);
		list_node = list_node->next;
	}
	return (FALSE);
}

/*
	get_env_value()		- Get the environment value
	
	return				- Environment variable's value

	env_list			- Current env list (t_env_list)
	key					- Environment variable name

	list_node			- Node for t_env_list in iterating linked list
	env_node			- Actual node for t_list content
*/
char	*get_env_value(t_env_list *env_list, char *key)
{
	t_list		*list_node;
	t_env_node	*env_node;

	list_node = env_list->head_node;
	while (list_node != NULL)
	{
		env_node = list_node->content;
		if (ft_strcmp(env_node->key, key) == 0)
			return (env_node->value);
		list_node = list_node->next;
	}
	return (NULL);
}

/*
	replace_env_value()	- Replace current environment variable value
	
	return				- void

	env_list			- Current env list (t_env_list)
	key					- Environment variable name
	new_value			- Value for replacing environment variable

	list_node			- Node for t_env_list in iterating linked list
	env_node			- Actual node for t_list content
*/
void	replace_env_value(t_env_list *env_list, char *key, char *new_value)
{
	t_list		*list_node;
	t_env_node	*env_node;

	list_node = env_list->head_node;
	while (list_node != NULL)
	{
		env_node = list_node->content;
		if (ft_strcmp(env_node->key, key) == 0)
		{
			free(env_node->value);
			env_node->value = ft_strdup(new_value);
			return ;
		}
		list_node = list_node->next;
	}
	env_node = malloc(sizeof(t_env_node));
	env_node->key = ft_strdup(key);
	env_node->value = ft_strdup(new_value);
	ft_lstadd_back(&env_list->head_node, ft_lstnew(env_node));
}

/*
	get_envp_in_list()	- Convert t_env_list into (char **envp)
	
	return				- Current environment variables (char **)

	env_list			- Current env list (t_env_list)

	idx					- Index for (char **envp)
	list_size			- Current env list size
	envp				- Result converting from t_env_list to char **
	list_node			- Node for t_env_list in iterating linked list
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
	save_envp_in_env_list()		- Save envp by linked list 
									in 3 ways line, key and value.
	
	return						- void

	envp						- main function's envp
	env_list					- linked list which is envp saved in

	idx							- index of envp
	kv_pair						- tokens split by '='
	env_node					- new node for env_list
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
		env_node->key = ft_strdup(kv_pair[0]);
		env_node->value = ft_strdup(kv_pair[1]);
		ft_lstadd_back(&env_list->head_node, ft_lstnew(env_node));
		free_all(kv_pair);
		idx += 1;
	}
}
