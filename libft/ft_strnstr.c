/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 17:09:42 by joonhan           #+#    #+#             */
/*   Updated: 2022/04/01 13:10:52 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *small, size_t len)
{
	size_t	i;
	size_t	cnt;
	size_t	small_len;

	small_len = ft_strlen((char *)small);
	if (small_len == 0)
		return ((char *)(big));
	i = 0;
	cnt = 0;
	while (i < len && *(big + i) != '\0')
	{
		while (*(big + i + cnt) == *(small + cnt) && *(big + i + cnt) != '\0')
		{
			if (i + cnt == len)
				break ;
			cnt += 1;
		}	
		if (cnt == small_len)
			break ;
		i += 1;
		cnt = 0;
	}
	if (cnt != small_len)
		return (NULL);
	return ((char *)(big + i));
}
