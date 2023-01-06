/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:17:59 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:18:02 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	init_parse_info(t_parse_info *parse_info)
{
	parse_info->is_command_found = FALSE;
	parse_info->is_redirection_found = FALSE;
	parse_info->is_heredoc_found = FALSE;
}

static t_bool	is_redir_not_heredoc(t_token_node *curr_token)
{
	if (curr_token->type == REDIR_LEFT)
		return (TRUE);
	if (curr_token->type == REDIR_RIGHT)
		return (TRUE);
	if (curr_token->type == REDIR_APPEND)
		return (TRUE);
	return (FALSE);
}

static void	process_word(t_token_node *curr_token, t_parse_info *parse_info)
{
	if (parse_info->is_redirection_found == TRUE)
	{
		curr_token->type = FILE_NAME;
		parse_info->is_redirection_found = FALSE;
	}
	else if (parse_info->is_command_found == FALSE && \
			parse_info->is_heredoc_found == FALSE)
	{
		curr_token->type = COMMAND;
		parse_info->is_command_found = TRUE;
	}
	else if (parse_info->is_heredoc_found == TRUE)
	{
		curr_token->type = LIMITER;
		parse_info->is_heredoc_found = FALSE;
	}
	else
		curr_token->type = ARGUMENT;
}

/*
	1. WORD 인 경우
		- 1-1. redirection 뒤는 무조건 파일 이름이 온다.
		- 1-2. command 를 처음 발견한 경우
			- command 는 redirection 보다 우선 처리해야 한다.
			- ex) ls << eof 
		- 1-3. HEREDOC 뒤에 오는 WORD 는 LIMITER 이다.
		- 1-4. 그 외의 경우에는 모두 ARGUMENT 

	2. <, >, >> 을 받았을 때

	3. << HEREDOC을 받았을 때
		- ex1). ls << eof cat
		- ex2). ls cat << eof

	4. 파이프를 받았을 때, 모두 False로 초기화
*/
void	parsing(t_token *token_list)
{
	t_list			*curr_node;
	t_token_node	*curr_token;
	t_parse_info	parse_info;

	init_parse_info(&parse_info);
	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		curr_token = (t_token_node *)curr_node->content;
		if (curr_token->type == WORD)
			process_word(curr_token, &parse_info);
		else if (is_redir_not_heredoc(curr_token) == TRUE && \
				parse_info.is_redirection_found == FALSE)
			parse_info.is_redirection_found = TRUE;
		else if (curr_token->type == REDIR_HEREDOC)
			parse_info.is_heredoc_found = TRUE;
		else if (curr_token->type == PIPE)
			init_parse_info(&parse_info);
		curr_node = curr_node->next;
	}
}
