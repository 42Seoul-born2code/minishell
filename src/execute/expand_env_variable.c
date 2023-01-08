/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_variable.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:17:50 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/08 20:25:08 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static void	make_env_word(char *input, int *idx, int start, \
							t_word_list *word_list)
{
	int		word_length;
	char	*env_word;

	word_length = *idx - start;
	env_word = malloc(sizeof(char) * (word_length + 1));
	ft_memcpy(env_word, &input[start], word_length);
	ft_lstadd_back(&word_list->head_node, \
			ft_lstnew(ft_strdup(getenv(env_word))));
	free(env_word);
}

static void	save_expanded_word(char *input, int *idx, t_word_list *word_list)
{
	int				start;

	*idx += 1;
	if (ft_isdigit(input[*idx]) == TRUE)
	{
		*idx += 1;
		return ;
	}
	start = *idx;
	while (input[*idx] != '\0')
	{
		if (is_valid_variable_rule(input[*idx]) == FALSE)
			break ;
		if (is_operator(&input[*idx]) == TRUE || \
			is_whitespace(input[*idx]) == TRUE)
			break ;
		*idx += 1;
	}
	if (start == *idx)
		ft_lstadd_back(&word_list->head_node, \
				ft_lstnew(ft_strdup("$")));
	else
	{
		make_env_word(input, idx, start, word_list);
	}	
}

static void	save_word(char *input, int *idx, t_word_list *word_list)
{
	int				start;
	int				word_length;
	char			*buffer;

	start = *idx;
	while (input[*idx] != '\0' && input[*idx] != '$')
		*idx += 1;
	word_length = *idx - start;
	buffer = malloc(sizeof(char) * (word_length + 1));
	ft_memcpy(buffer, &input[start], word_length);
	ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup(buffer)));
	free(buffer);
}

char	*expand_env_variable(char *input)
{
	int				idx;
	char			*result;
	t_word_list		*word_list;

	idx = 0;
	word_list = malloc(sizeof(t_word_list));
	word_list->head_node = NULL;
	while (input[idx] != '\0')
	{
		if (input[idx] == '$')
			save_expanded_word(input, &idx, word_list);
		else
			save_word(input, &idx, word_list);
	}
	result = merge_word_list(word_list);
	free(word_list);
	return (result);
}
