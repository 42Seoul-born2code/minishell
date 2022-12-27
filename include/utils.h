#ifndef UTILS_H
# define UTILS_H

typedef struct s_env_list {
	t_list	*head_node;
}	t_env_list;

typedef struct s_env_node {
	char	*key;
	char	*value;
}	t_env_node;

char	**get_envp_in_list(t_env_list *curr_env);
void	save_envp_in_list(char **envp, t_env_list *curr_env);

#endif