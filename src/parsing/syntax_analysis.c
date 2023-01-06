/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analysis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:18:01 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:18:29 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "utils.h"

static t_bool	is_continuous_pipe(t_list *curr_node)
{
	t_list			*next_node;
	t_token_node	*curr_token;
	t_token_node	*next_token;

	next_node = curr_node->next;
	curr_token = curr_node->content;
	next_token = next_node->content;
	if (curr_token->type == PIPE && next_token->type == PIPE)
		return (TRUE);
	return (FALSE);
}

static t_bool	is_next_token_operator(t_list *curr_node)
{
	t_list			*next_node;
	t_token_node	*next_token;

	next_node = curr_node->next;
	next_token = next_node->content;
	if (is_operator(next_token->word) == TRUE)
		return (TRUE);
	return (FALSE);
}

static int	throw_syntax_error(t_token_node *curr_token)
{
	g_exit_code = 258;
	return (print_error(SYNTAX_ERROR, curr_token->word));
}

/*
	1. operator 다음에 아무 것도 없을 때
	2. 파이프 바로 뒤에 파이프가 왔을 때
	3. redirection 바로 뒤에 operator 가 왔는가?
	4. 파이프 앞에 아무 것도 없거나 redirection 만 있을 때
*/
int	syntax_analysis(t_token *token_list)
{
	t_list			*prev_node;
	t_list			*curr_node;
	t_token_node	*curr_token;

	prev_node = NULL;
	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		if (is_operator(curr_token->word) == TRUE)
		{
			if (curr_token->type == PIPE && prev_node == NULL)
				return (throw_syntax_error(curr_token));
			else if (curr_node->next == NULL)
				return (throw_syntax_error(curr_token));
			else if (is_continuous_pipe(curr_node) == TRUE)
				return (throw_syntax_error(curr_token));
			else if (curr_token->type != PIPE && \
					is_next_token_operator(curr_node) == TRUE)
				return (throw_syntax_error(curr_node->next->content));
		}
		prev_node = curr_node;
		curr_node = curr_node->next;
	}
	return (SYNTAX_OK);
}
