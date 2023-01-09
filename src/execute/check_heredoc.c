/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:17:39 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/06 16:17:49 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "minishell.h"

static void	do_child_process(int heredoc_idx, char *limiter)
{
	int		fd;
	char	*input;
	char	*expand_result;

	fd = get_heredoc_file_fd(heredoc_idx, WRITE_MODE);
	while (TRUE)
	{
		change_heredoc_signal_child();
		input = readline(HEREDOC_PROMPT);
		if (ft_strcmp(input, limiter) == 0)
			break ;
		expand_result = expand_env_variable(input);
		ft_putstr_fd(expand_result, fd);
		ft_putstr_fd("\n", fd);
	}
	close(fd);
	exit(EXIT_SUCCESS);
}

void	get_user_input(char *limiter, int heredoc_idx)
{	
	pid_t	pid;

	change_heredoc_signal_parent();
	pid = fork();
	if (pid == CHILD_PROCESS)
		do_child_process(heredoc_idx, limiter);
	else
	{
		waitpid(pid, &g_exit_code, 0);
		g_exit_code = WEXITSTATUS(g_exit_code);
	}
}

void	check_heredoc(t_token *token_list)
{
	int				idx;
	t_list			*curr_node;
	t_token_node	*limiter_node;
	t_token_node	*curr_token;

	echoctl_off();
	idx = 0;
	curr_node = token_list->head_node;
	g_exit_code = 0;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		if (curr_token->type == REDIR_HEREDOC)
		{
			if (WIFSIGNALED(g_exit_code))
				break ;
			limiter_node = curr_node->next->content;
			get_user_input(limiter_node->word, idx);
			idx += 1;
		}
		curr_node = curr_node->next;
	}
	echoctl_on();
}
