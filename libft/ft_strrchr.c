/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 11:39:35 by joonhan           #+#    #+#             */
/*   Updated: 2022/03/30 15:15:09 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *src, int c)
{
	char	temp;
	char	*find;
	size_t	src_len;
	size_t	i;

	temp = (char) c;
	find = NULL;
	src_len = ft_strlen(src);
	i = 0;
	while (i <= src_len)
	{
		if (*(src + i) == temp)
			find = (char *)src + i;
		i += 1;
	}
	return (find);
}
