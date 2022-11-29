/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 14:17:05 by joonhan           #+#    #+#             */
/*   Updated: 2022/03/30 15:13:08 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *src, int c, size_t n)
{
	size_t			i;
	unsigned char	*find;
	unsigned char	*src_temp;
	unsigned char	temp;

	if (n == 0)
		return (NULL);
	i = 0;
	find = NULL;
	src_temp = (unsigned char *)src;
	temp = (unsigned char)c;
	while (i < n)
	{
		if (*(src_temp + i) == temp)
		{
			find = (src_temp + i);
			break ;
		}
		i += 1;
	}
	return (find);
}
