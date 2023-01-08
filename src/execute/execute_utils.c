/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:17:48 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/08 11:58:33 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

t_bool	is_redirection(t_token_node *curr_token)
{
	t_meta	type;

	type = curr_token->type;
	if (type == REDIR_APPEND || type == REDIR_HEREDOC || \
		type == REDIR_RIGHT || type == REDIR_LEFT)
		return (TRUE);
	return (FALSE);
}

char	**merge_arguments(t_list *curr_node)
{
	int				idx;
	int				argv_count;
	char			**cmd_argv;
	t_token_node	*curr_token;

	idx = 0;
	argv_count = count_argv(curr_node);
	cmd_argv = malloc((sizeof(char *)) * (argv_count + 1));
	while (idx < argv_count)
	{
		curr_token = curr_node->content;
		if (curr_token->type == PIPE)
			break ;
		if (is_redirection(curr_token) == TRUE && curr_node->next != NULL)
			curr_node = curr_node->next;
		else if (curr_token->type == ARGUMENT || curr_token->type == COMMAND)
		{
			cmd_argv[idx] = ft_strjoin(curr_token->word, "");
			idx += 1;
		}
		curr_node = curr_node->next;
	}
	cmd_argv[argv_count] = NULL;
	return (cmd_argv);
}

int	count_argv(t_list *curr_node)
{
	int				argv_count;
	t_token_node	*curr_token;

	argv_count = 0;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		if (curr_token->type == PIPE)
			break ;
		if (is_redirection(curr_token) == TRUE && curr_node->next != NULL)
			curr_node = curr_node->next;
		else if (curr_token->type == ARGUMENT || curr_token->type == COMMAND)
			argv_count += 1;
		curr_node = curr_node->next;
	}
	return (argv_count);
}

void	close_pipes(int pipe_fd[2])
{
	close(pipe_fd[READ]);
	close(pipe_fd[WRITE]);
}
