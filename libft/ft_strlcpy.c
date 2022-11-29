/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 13:38:51 by joonhan           #+#    #+#             */
/*   Updated: 2022/04/10 10:36:18 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char	*dest, const char	*src, size_t destsize)
{
	size_t	src_len;

	src_len = ft_strlen(src);
	if (src_len + 1 < destsize)
		ft_memcpy(dest, src, src_len + 1);
	else if (destsize != 0)
	{
		ft_memcpy(dest, src, destsize - 1);
		*(dest + destsize - 1) = '\0';
	}
	return (src_len);
}
