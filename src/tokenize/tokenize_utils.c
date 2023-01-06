/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:17:47 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:17:51 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenize.h"

t_bool	is_operator(char *str);
t_bool	is_quote(char c);
t_meta	get_meta_type(char *str);
void	get_operator_type(t_token_node *node, char *line, \
								int *idx, int *length);

t_bool	is_operator(char *str)
{
	if (ft_strncmp(str, "<<", 2) == 0 || \
	ft_strncmp(str, ">>", 2) == 0)
	{
		return (TRUE);
	}	
	if (*str == '|' || *str == '<' || *str == '>')
	{
		return (TRUE);
	}
	return (FALSE);
}

t_bool	is_quote(char c)
{
	if (c == '\'' || c == '\"')
	{
		return (TRUE);
	}
	return (FALSE);
}

t_meta	get_meta_type(char *str)
{
	if (is_whitespace(*str) == TRUE)
		return (WHITESPACE);
	if (*str == '|')
		return (PIPE);
	if (ft_strncmp(str, "<<", 2) == 0)
		return (REDIR_HEREDOC);
	if (*str == '<')
		return (REDIR_LEFT);
	if (ft_strncmp(str, ">>", 2) == 0)
		return (REDIR_APPEND);
	if (*str == '>')
		return (REDIR_RIGHT);
	if (*str == '|')
		return (PIPE);
	else
		return (WORD);
}

void	get_operator_type(t_token_node *node, char *line, \
								int *idx, int *length)
{
	node->type = get_meta_type(&line[*idx]);
	if (node->type == REDIR_HEREDOC || node->type == REDIR_APPEND)
	{
		*length = 2;
		*idx += 2;
	}
	else
	{
		*length = 1;
		*idx += 1;
	}
}
