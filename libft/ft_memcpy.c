/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@studnet.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 13:57:24 by joonhan           #+#    #+#             */
/*   Updated: 2022/08/24 16:46:51 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, ssize_t n)
{
	ssize_t			i;
	unsigned char	*dest_temp;
	unsigned char	*src_temp;

	if (n == 0 || dest == src)
		return (dest);
	i = 0;
	dest_temp = (unsigned char *)dest;
	src_temp = (unsigned char *)src;
	while (i < n)
	{
		*(dest_temp + i) = *(src_temp + i);
		i += 1;
	}
	*(dest_temp + i) = '\0';
	return (dest);
}
