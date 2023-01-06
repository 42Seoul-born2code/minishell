/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:18:09 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:18:15 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

/*

	TEST CASES

	echo '"hello"'
	-> "hello"
	
	echo "hello '$USER'"
	-> hello 'joonhan'

	echo hello'world'$USER
	-> helloworldjoonhan
	
	echo hello"'world'"joonpark
	-> hello'world'joonpark

	echo hello$USER
	-> hellojoonhan

	echo hello'"',dear:'"'$USER
	-> hello",dear:"joonhan

	'b''a'"s""h"
	-> bash

*/

/*
	1-1. 끝나는 따옴표 위치 파악
	1-2. 따옴표 제외한 문자열 복사
*/
static char	*save_quoted_word(t_token_node *curr_token, int *idx)
{
	int		start;
	char	start_quote;
	char	*word;

	start_quote = curr_token->word[*idx];
	word = NULL;
	*idx += 1;
	start = *idx;
	while (curr_token->word[*idx] != '\0' && \
			curr_token->word[*idx] != start_quote)
		*idx += 1;
	if (start != *idx)
	{
		word = copy_str_length(start, curr_token->word, *idx - start);
		*idx += 1;
	}
	else
		*idx += 1;
	return (word);
}

static char	*save_unquoted_word(t_token_node *curr_token, int *idx)
{
	int		start;
	char	*word;

	start = *idx;
	word = NULL;
	while (curr_token->word[*idx] != '\0')
	{
		if (is_quote(curr_token->word[*idx]) == TRUE)
			break ;
		*idx += 1;
	}
	if (start != *idx)
		word = copy_str_length(start, curr_token->word, *idx - start);
	else
		*idx += 1;
	return (word);
}

/*
	1. 따옴표를 만난 경우
	2. 따옴표가 아닌 경우
	3. 따옴표 이전 문자열과 join
*/
static char	*join_quote_removed_word(t_token_node *curr_token)
{
	int		idx;
	char	*buffer;
	char	*prev_word;
	char	*result_word;

	idx = 0;
	buffer = NULL;
	result_word = NULL;
	while (curr_token->word[idx] != '\0')
	{
		prev_word = NULL;
		if (is_quote(curr_token->word[idx]) == TRUE)
			prev_word = save_quoted_word(curr_token, &idx);
		else
			prev_word = save_unquoted_word(curr_token, &idx);
		if (result_word != NULL)
		{
			buffer = ft_strdup(result_word);
			free(result_word);
		}
		result_word = ft_strjoin(buffer, prev_word);
		free(buffer);
		free(prev_word);
	}
	return (result_word);
}

void	quote_removal(t_token *token_list)
{
	char			*result_word;
	t_list			*curr_node;
	t_token_node	*curr_token;

	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		result_word = join_quote_removed_word(curr_token);
		free(curr_token->word);
		curr_token->word = result_word;
		curr_node = curr_node->next;
	}
}
