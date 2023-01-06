/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_whitespace.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:18:12 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:18:56 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	slice_and_save_str(char *str, int start, int idx, char **result)
{
	char	*buffer;
	char	*prev_word;

	buffer = ft_substr(str, start, idx - start);
	prev_word = ft_strdup(*result);
	if (*result != NULL)
		free(*result);
	*result = ft_strjoin(prev_word, buffer);
	free(prev_word);
	free(buffer);
	if (str[idx] != '\0')
	{
		prev_word = ft_strdup(*result);
		free(*result);
		*result = ft_strjoin(prev_word, " ");
		free(prev_word);
	}
}

char	*remove_whitespace(char *str)
{
	int		idx;
	int		start;
	char	*buffer;
	char	*result;

	if (str == NULL)
		return (NULL);
	idx = 0;
	result = NULL;
	buffer = ft_strtrim(str, " \n\t\f\v\r");
	while (str[idx] != '\0')
	{
		start = idx;
		while (is_whitespace(buffer[idx]) == FALSE && buffer[idx] != '\0')
			idx += 1;
		if (start == idx)
		{
			idx += 1;
			continue ;
		}
		slice_and_save_str(buffer, start, idx, &result);
	}
	free(buffer);
	free(str);
	return (result);
}
