/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:18:00 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/08 11:54:14 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	process_redirection(t_list *curr_node, t_redirect *redirect_info)
{
	t_token_node	*curr_token;

	curr_token = curr_node->content;
	if (curr_token->type == REDIR_RIGHT || curr_token->type == REDIR_APPEND)
	{
		if (curr_token->type == REDIR_RIGHT)
			redirect_right(curr_node, redirect_info);
		else if (curr_token->type == REDIR_APPEND)
			redirect_append(curr_node, redirect_info);
	}
	else
	{
		if (curr_token->type == REDIR_LEFT)
			redirect_left(curr_node, redirect_info);
		else if (curr_token->type == REDIR_HEREDOC)
			redirect_heredoc(redirect_info);
	}
}
