/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:18:54 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/11 14:49:18 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include "execute.h"

# define INVALID_OPTION			"❌ Error: invalid option: "
# define NOT_NUMERIC_ARGUMENT	"❌ Error: numeric argument required: "
# define TOO_MANY_ARGUMNET		"❌ Error: too many arguments. "
# define VARIABLE_IS_UNSET		"❌ Error: variable is unset. "
# define NOT_VALID_IDENTIFIER	"❌ Error: not a valid identifier: "
# define NO_SUCH_FILE_OR_DIR	"❌ Error: No such file or directory: "

typedef enum e_kv_pair{
	KEY,
	VALUE
}	t_kv_pair;

typedef struct e_dir_info{
	char		*curr_path;
	char		*abs_path;
	struct stat	buf;
	struct stat	abs_buf;
}	t_dir_info;

// builtin_utils.c
t_bool	is_valid_variable_name(char *str);
void	ft_lstadd_env_node(t_env_list *env_list, char *key, char *value);
int		check_option(char *argv);

// ft_cd.c
int		ft_cd(char **argv, t_env_list *env_list);

// ft_cd_utils.c
int		try_to_move_directories(int idx, char *argv, \
									char **paths, t_env_list *env_list);
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
int		ft_exit(char **argv, t_env_list *env_list, t_command_type command_type);

#endif