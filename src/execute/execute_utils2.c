/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:17:49 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/08 13:11:00 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	init_cmd_info(t_cmd_info *cmd_info, t_init type)
{
	if (type == INIT)
	{
		cmd_info->cmd_name = NULL;
		cmd_info->cmd_argv = NULL;
	}
	else if (type == FREE)
	{
		if (cmd_info->cmd_name != NULL)
		{
			free(cmd_info->cmd_name);
			cmd_info->cmd_name = NULL;
		}
		if (cmd_info->cmd_argv != NULL)
		{
			free_all(cmd_info->cmd_argv);
			cmd_info->cmd_argv = NULL;
		}
	}
}

void	init_redirect_info(t_redirect *redirect_info)
{
	redirect_info->infile = NONE;
	redirect_info->outfile = NONE;
	redirect_info->type = NORMAL;
}

static void	close_pipe_and_dup2(int pipe_fd[2], int type)
{
	if (type == CHILD_PROCESS)
	{
		close(pipe_fd[READ]);
		dup2(pipe_fd[WRITE], STDOUT_FILENO);
		close(pipe_fd[WRITE]);
	}
	else
	{
		close(pipe_fd[WRITE]);
		dup2(pipe_fd[READ], STDIN_FILENO);
		close(pipe_fd[READ]);
	}
}

static int	child_process(t_cmd_info *cmd_info, \
		t_env_list *env_list, t_redirect redirect_info)
{
	pid_t	pid;
	int		pipe_fd[2];

	pipe(pipe_fd);
	change_signal();
	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		if (redirect_info.type == OUTFILE)
			close_pipes(pipe_fd);
		else
			close_pipe_and_dup2(pipe_fd, CHILD_PROCESS);
		if (is_builtin_function(cmd_info->cmd_name) == TRUE)
			exit(execute_builtin_function(cmd_info->cmd_name, \
				cmd_info->cmd_argv, env_list, MULTI_COMMAND));
		else
			execute_cmd(cmd_info->cmd_name, cmd_info->cmd_argv, \
						redirect_info, env_list);
	}
	else
	{
		close_pipe_and_dup2(pipe_fd, PARENT_PROCESS);
		waitpid(pid, NULL, WNOHANG);
	}
	return (1);
}

int	process_tokens(t_list *curr_node, t_cmd_info *cmd_info, \
				t_redirect *redirect_info, t_env_list *env_list)
{
	int				process_count;
	t_token_node	*curr_token;

	process_count = 0;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		if (curr_token->type == COMMAND)
		{
			cmd_info->cmd_name = ft_strdup(curr_token->word);
			cmd_info->cmd_argv = merge_arguments(curr_node);
		}
		else if (is_redirection(curr_token) == TRUE)
			process_redirection(curr_node, redirect_info);
		else if (curr_token->type == PIPE)
		{
			process_count += child_process(cmd_info, env_list, *redirect_info);
			init_cmd_info(cmd_info, FREE);
			init_redirect_info(redirect_info);
		}
		curr_node = curr_node->next;
	}
	return (process_count);
}
