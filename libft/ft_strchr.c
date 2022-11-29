/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 17:26:30 by joonhan           #+#    #+#             */
/*   Updated: 2022/03/30 15:42:25 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *src, int c)
{
	char	temp;
	char	*find;
	size_t	i;
	size_t	src_len;

	temp = (char) c;
	find = NULL;
	i = 0;
	src_len = ft_strlen(src);
	while (i <= src_len)
	{
		if (*(src + i) == temp)
		{
			find = (char *)(src + i);
			break ;
		}
		i += 1;
	}
	return (find);
}
