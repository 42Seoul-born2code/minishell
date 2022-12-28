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

# define SYNTAX_ERROR	"Error: Syntax Error"

typedef enum e_kv_pair{
	KEY,
	VALUE
}	t_kv_pair;

// builtin_utils.c
t_bool	is_valid_variable_name(char *str);

// check_option.c
int		check_option(char *argv);

// ft_echo.c
int		ft_echo(char **argv);

// ft_pwd.c
int		ft_pwd(char **argv);

// ft_env.c
int		ft_env(char **argv, t_env_list *env_list);

// ft_export.c
int		ft_export(char **argv, t_env_list *env_list);

// ft_unset.c
int		ft_unset(char **argv, t_env_list *env_list);

#endif