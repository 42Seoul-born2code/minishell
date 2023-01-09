/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_builtin_function.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:17:54 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/06 16:18:08 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execute.h"

t_bool	is_builtin_function(char *word)
{
	if (ft_strcmp(word, "echo") == 0)
		return (TRUE);
	else if (ft_strcmp(word, "cd") == 0)
		return (TRUE);
	else if (ft_strcmp(word, "pwd") == 0)
		return (TRUE);
	else if (ft_strcmp(word, "export") == 0)
		return (TRUE);
	else if (ft_strcmp(word, "unset") == 0)
		return (TRUE);
	else if (ft_strcmp(word, "env") == 0)
		return (TRUE);
	else if (ft_strcmp(word, "exit") == 0)
		return (TRUE);
	return (FALSE);
}

t_bool	is_command_builtin_function(t_token *token_list)
{
	t_list			*curr_node;
	t_token_node	*curr_token;

	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		if (curr_token->type == COMMAND)
		{
			if (is_builtin_function(curr_token->word) == TRUE)
				return (TRUE);
		}
		curr_node = curr_node->next;
	}
	return (FALSE);
}

void	process_builtin_function(t_token *token_list, t_env_list *env_list)
{
	int				origin_fd[2];
	t_cmd_info		cmd_info;
	t_redirect		redirect_info;

	save_origin_fd(origin_fd);
	init_cmd_info(&cmd_info, INIT);
	init_redirect_info(&redirect_info);
	redirect_info.heredoc_file_num = 0;
	process_tokens(token_list->head_node, \
						&cmd_info, &redirect_info, env_list);
	if ((redirect_info.type != NORMAL && g_exit_code == EXIT_FAILURE) == FALSE)
	{
		g_exit_code = execute_builtin_function(cmd_info.cmd_name, \
			cmd_info.cmd_argv, env_list, SIMPLE_COMMAND);
	}
	init_cmd_info(&cmd_info, FREE);
	rollback_origin_fd(origin_fd);
	delete_heredoc_file(redirect_info.heredoc_file_num);
}
