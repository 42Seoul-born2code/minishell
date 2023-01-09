/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:18:05 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/08 20:20:22 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "expansion.h"

t_bool	is_valid_variable_rule(char c)
{
	if (ft_isalnum(c) == TRUE || c == '_')
		return (TRUE);
	return (FALSE);
}

char	*copy_str_length(int start, const char *src, int word_length)
{
	char	*copied_str;

	copied_str = malloc(sizeof(char) * (word_length + 1));
	ft_memcpy(copied_str, &src[start], word_length);
	return (copied_str);
}

void	save_before_env_variable(\
char *word, int *idx, t_word_list *word_list, t_quote type)
{
	int		start;
	char	*buffer;
	t_bool	is_stopable;

	start = *idx;
	is_stopable = FALSE;
	while (word[*idx] != '\0')
	{
		if (type == NOT_QUOTED)
		{
			if (word[*idx] == '\'' || \
				word[*idx] == '\"' || word[*idx] == '$')
			is_stopable = TRUE;
		}
		else if (word[*idx] == '\"' || word[*idx] == '$')
			is_stopable = TRUE;
		if (is_stopable == TRUE)
			break ;
		*idx += 1;
	}
	buffer = copy_str_length(start, word, *idx - start);
	ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup(buffer)));
	free(buffer);
}
