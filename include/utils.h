#ifndef UTILS_H
# define UTILS_H

# include "libft.h"
# include "tokenize.h"

# define SYNTAX_ERROR	"Error: Syntax Error: "
# define NOT_EXISTED	"Error: No such file or directory: "
# define HOME_IS_UNSET	"Error: HOME not set"

typedef struct s_env_list {
	t_list	*head_node;
}	t_env_list;

typedef struct s_env_node {
	char	*key;
	char	*value;
}	t_env_node;

// env_utils.c
t_bool		is_env_existed(t_env_list *env_list, char *key);
char		*get_env_value(t_env_list *env_list, char *key);
void		replace_env_value(t_env_list *env_list, char *key, char *new_value);
char		**get_envp_in_list(t_env_list *env_list);
void		save_envp_in_env_list(char **envp, t_env_list *env_list);

// error.c
int			print_error(char *msg, char *arg);

#endif