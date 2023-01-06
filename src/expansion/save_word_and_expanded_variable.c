/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_word_and_expanded_variable.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:18:16 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:18:18 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static void	save_single_quoted_word(char *word, int *idx, \
									t_word_list *word_list)
{
	int		start;
	int		word_length;
	char	*buffer;

	ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("\'")));
	*idx += 1;
	start = *idx;
	while (word[*idx] != '\0' && word[*idx] != '\'')
		*idx += 1;
	if (start == *idx && word[*idx] == '\'')
	{
		ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("\'")));
		*idx += 1;
	}
	else if ((start == *idx && word[*idx] == '\0') == FALSE)
	{
		word_length = *idx - start;
		buffer = malloc(sizeof(char) * (word_length + 1));
		ft_memcpy(buffer, &word[start], word_length);
		ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup(buffer)));
		ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("\'")));
		free(buffer);
		*idx += 1;
	}
}

/*
	1. $ 만난 경우: 환경변수 확장해서 저장
	2. $ 만나기 전의 경우: 환경변수 만나기 전까지 문자열 저장
*/
//add_back_to_word_list(word_info, idx, word_list, QUOTED);
static void	add_back_to_word_list(t_word_info word_info, int *idx, \
								t_word_list *word_list, t_quote quote_type)
{
	char	*word;

	word = word_info.word;
	if (word[*idx] == '$')
	{
		save_expand_env_variable(word_info, idx, word_list, quote_type);
	}
	else
	{
		save_before_env_variable(word, idx, word_list, quote_type);
	}
}

static void	save_double_quoted_word(t_word_info word_info, int *idx, \
									t_word_list *word_list)
{
	char	*word;

	word = word_info.word;
	ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("\"")));
	*idx += 1;
	while (word[*idx] != '\0' && word[*idx] != '\"')
	{
		add_back_to_word_list(word_info, idx, word_list, QUOTED);
	}
	if (word[*idx] == '\"')
	{
		ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("\"")));
		*idx += 1;
	}
}

static void	save_unquoted_word(t_word_info word_info, int *idx, \
								t_word_list *word_list)
{
	char	*word;

	word = word_info.word;
	while (word[*idx] != '\0' && word[*idx] != '\"' && word[*idx] != '\'')
		add_back_to_word_list(word_info, idx, word_list, NOT_QUOTED);
}

t_word_list	*save_word_and_expanded_variable(
	t_token_node *curr_token, t_env_list *env_list)
{
	int				idx;
	t_word_list		*word_list;
	t_word_info		word_info;

	idx = 0;
	word_list = malloc(sizeof(t_word_list));
	word_list->head_node = NULL;
	word_info.word = curr_token->word;
	word_info.type = curr_token->type;
	word_info.env_list = env_list;
	while (word_info.word[idx] != '\0')
	{
		if (word_info.word[idx] == '\'')
			save_single_quoted_word(word_info.word, &idx, word_list);
		else if (word_info.word[idx] == '\"')
			save_double_quoted_word(word_info, &idx, word_list);
		else
			save_unquoted_word(word_info, &idx, word_list);
	}
	return (word_list);
}
