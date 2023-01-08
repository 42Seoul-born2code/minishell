/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:17:28 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/08 13:10:53 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execute.h"
#include "utils.h"
#include "mini_signal.h"

void	throw_error(char *msg)
{
	print_error(msg, NULL);
	exit(EXIT_FAILURE);
}

static void	execute_simple_command(t_cmd_info *cmd_info, \
	t_redirect redirect_info, t_env_list *env_list, int origin_fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		if (is_builtin_function(cmd_info->cmd_name) == TRUE)
			exit(execute_builtin_function(cmd_info->cmd_name, \
			cmd_info->cmd_argv, env_list, SIMPLE_COMMAND));
		execute_cmd(cmd_info->cmd_name, cmd_info->cmd_argv, \
					redirect_info, env_list);
	}
	else
	{
		waitpid(pid, &g_exit_code, 0);
		if (WIFSIGNALED(g_exit_code))
			g_exit_code = 128 + WTERMSIG(g_exit_code);
		else
			g_exit_code = WEXITSTATUS(g_exit_code);
		rollback_origin_fd(origin_fd);
		init_cmd_info(cmd_info, FREE);
		delete_heredoc_file(redirect_info.heredoc_file_num);
	}
}

/*
	종료하는 케이스
	1. 히어독에서 시그널 받아서 종료되었을 때
	2. 리다이렉션은 들어왔지만, 파일을 실제로 못 열었을 때
*/
void	fork_process(t_token *token_list, t_env_list *env_list)
{
	int				origin_fd[2];
	t_cmd_info		cmd_info;
	t_redirect		redirect_info;

	change_signal();
	save_origin_fd(origin_fd);
	init_cmd_info(&cmd_info, INIT);
	init_redirect_info(&redirect_info);
	redirect_info.heredoc_file_num = 0;
	process_tokens(token_list->head_node, \
						&cmd_info, &redirect_info, env_list);
	if ((g_exit_code != 0 && redirect_info.type == HEREDOC) || \
		(g_exit_code == 1 && redirect_info.type != NORMAL))
	{
		init_cmd_info(&cmd_info, FREE);
		rollback_origin_fd(origin_fd);
		return ;
	}
	execute_simple_command(&cmd_info, redirect_info, env_list, origin_fd);
}
