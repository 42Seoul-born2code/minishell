/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 15:32:07 by joonhan           #+#    #+#             */
/*   Updated: 2022/03/30 15:43:29 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t destsize)
{
	size_t	dest_len;
	size_t	dest_idx;
	size_t	src_len;
	size_t	src_idx;

	dest_len = ft_strlen(dest);
	dest_idx = dest_len;
	src_len = ft_strlen(src);
	src_idx = 0;
	if (destsize == 0 || destsize < dest_len + 1)
		return (destsize + src_len);
	while (*(src + src_idx) != '\0')
	{
		if ((dest_idx + 1) == destsize)
			break ;
		*(dest + dest_idx) = *(src + src_idx);
		dest_idx += 1;
		src_idx += 1;
	}
	*(dest + dest_idx) = '\0';
	return (dest_len + src_len);
}
