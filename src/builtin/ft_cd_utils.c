/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:19:42 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:19:56 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <dirent.h>

static t_bool	is_dir_accessible(char *path)
{
	DIR	*p_dir;

	p_dir = opendir(path);
	if (p_dir == NULL)
		return (FALSE);
	else
	{
		closedir(p_dir);
		return (TRUE);
	}
}

void	save_curr_pwd(t_dir_info dir_info, t_env_list *env_list)
{
	dir_info.curr_path = getcwd(NULL, BUFSIZ);
	replace_env_value(env_list, "PWD", dir_info.curr_path);
	free(dir_info.curr_path);
}

static int	check_path(char *path, char *argv)
{
	if (is_dir_accessible(path) == TRUE)
	{
		chdir(path);
		return (EXIT_SUCCESS);
	}
	else
		return (print_error(PERMISSION_DENIED, argv));
}

int	try_to_move_directories(int idx, char *argv, \
									char **paths, t_env_list *env_list)
{
	int			result;
	t_dir_info	dir_info;

	result = EXIT_SUCCESS;
	while (paths[++idx] != NULL && result == EXIT_SUCCESS)
	{
		dir_info.abs_path = ft_strjoin("/", paths[idx]);
		if (stat(paths[idx], &dir_info.buf) == 0 && \
			S_ISDIR(dir_info.buf.st_mode))
		{
			result = check_path(paths[idx], argv);
		}
		else if (stat(dir_info.abs_path, &dir_info.abs_buf) == 0 && \
				S_ISDIR(dir_info.abs_buf.st_mode))
		{
			result = check_path(dir_info.abs_path, argv);
		}
		else
			result = print_error(NOT_EXISTED, argv);
		free(dir_info.abs_path);
		if (result == EXIT_SUCCESS)
			save_curr_pwd(dir_info, env_list);
	}
	return (result);
}
