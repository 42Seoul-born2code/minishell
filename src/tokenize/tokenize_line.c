/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:17:49 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:17:52 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenize.h"
#include "utils.h"

static void	save_token(t_token *token_list, t_token_node *node, \
						char *line, int length)
{
	char	*word;

	word = malloc(sizeof(char) * (length + 1));
	ft_memcpy(word, line, length);
	node->word = word;
	ft_lstadd_back(&token_list->head_node, ft_lstnew(node));
}

// 따옴표로 묶인 문자들을 연속으로 만났을 때 하나의 word 로 처리하기 위한 반복문
static t_bool	is_quote_closed(char *line, int *i)
{
	char	quote_type;
	t_bool	result;

	result = FALSE;
	if (is_quote(line[*i]) == FALSE)
	{
		*i += 1;
		return (TRUE);
	}
	quote_type = line[*i];
	*i += 1;
	while (line[*i] != '\0')
	{
		if (quote_type == line[*i])
		{
			result = TRUE;
			*i += 1;
			break ;
		}
		*i += 1;
	}
	return (result);
}

static int	get_word_length(t_token_node *node, char *line, int *i, int start)
{
	while (line[*i] != '\0')
	{
		if (is_quote(line[*i]) == TRUE)
		{
			if (is_quote_closed(line, i) == FALSE)
				return (ERROR);
			else
				continue ;
		}
		if (is_whitespace(line[*i]) == TRUE || is_operator(&line[*i]) == TRUE)
		{
			break ;
		}
		*i += 1;
	}
	node->type = WORD;
	return (*i - start);
}

/*
	LINE77	: CASE1. operator 는 바로 저장
	LINE79	: CASE2. word 는 저장 시도
	LINE82	: CASE3. 따옴표가 제대로 닫히지 않은 경우 에러 반환
*/
int	add_token_lstback(t_token *token_list, char *line, int *idx)
{
	int				start;
	int				word_length;
	t_token_node	*token_node;

	start = *idx;
	token_node = malloc(sizeof(t_token_node));
	if (is_operator(&line[*idx]) == TRUE)
		get_operator_type(token_node, line, idx, &word_length);
	else
	{
		word_length = get_word_length(token_node, line, idx, start);
		if (word_length == ERROR)
		{
			free(token_node);
			return (EXIT_ERROR);
		}
	}
	save_token(token_list, token_node, &line[start], word_length);
	return (EXIT_SUCCESS);
}

/*
	LINE102	: CASE1. whitespace가 아닌 경우 토큰 저장 시도
*/
int	tokenize_line(char *line, t_token *token_list)
{
	int	idx;

	idx = 0;
	while (line[idx] != '\0')
	{
		if (is_whitespace(line[idx]) == FALSE)
		{
			if (add_token_lstback(token_list, line, &idx) == EXIT_ERROR)
			{
				g_exit_code = print_error(NOT_CLOSED_QUOTE, line);
				return (EXIT_ERROR);
			}
		}
		else
			idx += 1;
	}
	return (EXIT_SUCCESS);
}
