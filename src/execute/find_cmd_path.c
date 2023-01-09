/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:16:43 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/08 11:51:32 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execute.h"

static char	**get_path_env(char *str)
{
	if (str == NULL)
		return (NULL);
	else
		return (ft_split(str, ':'));
}

static t_bool	is_command_directory(char *cmd)
{
	struct stat	buf;

	stat(cmd, &buf);
	if (S_ISDIR(buf.st_mode))
	{
		print_error(cmd, "is a directory");
		g_exit_code = 126;
		return (TRUE);
	}
	return (FALSE);
}

static char	*get_cmd_path(char *cmd, char **path_env)
{
	int		idx;
	char	*path;
	char	*cmd_path;

	idx = 0;
	path = NULL;
	while (path_env[idx] != NULL)
	{
		path = ft_strjoin(path_env[idx], "/");
		cmd_path = ft_strjoin(path, cmd);
		free(path);
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		idx += 1;
	}
	return (NULL);
}

char	*find_cmd_path(char *cmd, t_env_list *env_list)
{
	char		*cmd_path;
	char		**path_env;

	if (ft_strlen(cmd) == 0)
		return (NULL);
	if (is_command_directory(cmd) == TRUE)
		return (NULL);
	if (access(cmd, F_OK | X_OK) == 0)
		return (ft_strdup(cmd));
	path_env = get_path_env(get_env_value(env_list, "PATH"));
	if (path_env == NULL)
		return (NULL);
	cmd_path = get_cmd_path(cmd, path_env);
	free_all(path_env);
	if (cmd_path == NULL)
		return (NULL);
	return (cmd_path);
}
