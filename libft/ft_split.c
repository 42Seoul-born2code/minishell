/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 13:18:29 by joonhan           #+#    #+#             */
/*   Updated: 2022/12/27 17:24:53 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_word_cnt(const char *src, char d)
{
	size_t	i;
	size_t	cnt;

	i = 0;
	cnt = 0;
	while (*(src + i) != '\0')
	{
		if (*(src + i) != d)
		{
			cnt += 1;
			while (*(src + i) != '\0' && *(src + i) != d)
				i += 1;
		}
		else
			i += 1;
	}
	return (cnt);
}

static char	*duplicate_word(const char *src, char d)
{
	size_t	i;
	char	*ptr;

	i = 0;
	while (*(src + i) != '\0' && *(src + i) != d)
		i += 1;
	ptr = (char *)malloc((i + 1) * sizeof(char));
	if (ptr == NULL)
		return (NULL);
	ft_strlcpy(ptr, src, i + 1);
	return (ptr);
}

char	**free_all(char **arr)
{
	size_t	i;

	i = 0;
	while (*(arr + i) != NULL)
	{
		free(*(arr + i));
		i += 1;
	}
	free(arr);
	return (NULL);
}

static char	**split_by_char(char const *s, char d, char **arr, size_t cnt)
{
	size_t	arr_idx;
	size_t	s_idx;

	arr_idx = 0;
	s_idx = 0;
	while (*(s + s_idx) != '\0' && arr_idx < cnt)
	{
		if (*(s + s_idx) != d)
		{
			*(arr + arr_idx) = duplicate_word((s + s_idx), d);
			if (*(arr + arr_idx++) == NULL)
				return (free_all(arr));
			while (*(s + s_idx) != '\0' && *(s + s_idx) != d)
				s_idx += 1;
		}
		else
			s_idx += 1;
	}
	return (arr);
}

char	**ft_split(char const *s, char d)
{
	char	**arr;
	size_t	word_cnt;

	if (!s)
		return (NULL);
	word_cnt = get_word_cnt(s, d);
	arr = (char **)malloc((word_cnt + 1) * sizeof(char *));
	if (arr == NULL)
		return (NULL);
	arr = split_by_char(s, d, arr, word_cnt);
	arr[word_cnt] = 0;
	return (arr);
}
