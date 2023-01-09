/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:17:41 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/08 15:11:18 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execute.h"
#include "libft.h"

void	execute_cmd(char *cmd_name, char **cmd_argv, \
					t_redirect redirect_info, t_env_list *env_list)
{
	char	*cmd_path;
	char	**envp;

	cmd_path = find_cmd_path(cmd_name, env_list);
	if (g_exit_code == 126)
		exit(126);
	if (redirect_info.heredoc_file_num > 0 && \
		cmd_name == NULL && cmd_path == NULL && redirect_info.type == NORMAL)
		exit(EXIT_SUCCESS);
	if (cmd_path == NULL && redirect_info.type == NORMAL)
	{
		print_error(COMMAND_NOT_FOUND, cmd_name);
		exit(ERROR_CODE_COMMAND_NOT_FOUND);
	}
	if (g_exit_code == EXIT_FAILURE && redirect_info.type != NORMAL)
		exit(g_exit_code);
	if (cmd_argv == NULL)
		exit(EXIT_SUCCESS);
	envp = get_envp_in_list(env_list);
	if (execve(cmd_path, cmd_argv, envp) == ERROR)
	{
		print_error(COMMAND_NOT_FOUND, cmd_path);
		exit(ERROR_CODE_COMMAND_NOT_FOUND);
	}
}
