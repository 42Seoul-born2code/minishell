#ifndef BUILTIN_H
# define BUILTIN_H

# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <fcntl.h>

# include "libft.h"
# include "utils.h"

typedef enum e_kv_pair{
	KEY,
	VALUE
}	t_kv_pair;

// builtin_utils.c
t_bool	is_valid_variable_name(char *str);

// check_option.c
int		check_option(char *argv);

// ft_cd.c
int		ft_cd(char **argv, t_env_list *env_list);

// ft_cd_utils.c
int		move_to_env_path(char *env_path, t_env_list *env_list);
char	*get_parent_directory(t_env_list *env_list);
char	*join_path(char **paths, int *idx);
t_bool	is_path_existed(char **curr_path, char **paths, int *idx);

// ft_echo.c
int		ft_echo(char **argv);

// ft_pwd.c
int		ft_pwd(char **argv, t_env_list *env_list);

// ft_env.c
int		ft_env(char **argv, t_env_list *env_list);

// ft_export.c
int		ft_export(char **argv, t_env_list *env_list);

// ft_unset.c
int		ft_unset(char **argv, t_env_list *env_list);

// ft_exit.c
void	ft_exit(char **argv, t_env_list *env_list);
#endif