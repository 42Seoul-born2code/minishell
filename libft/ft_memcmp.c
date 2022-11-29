/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 14:41:32 by joonhan           #+#    #+#             */
/*   Updated: 2022/03/30 15:13:35 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	int				result;
	size_t			i;
	unsigned char	*s1_temp;
	unsigned char	*s2_temp;

	if (n == 0)
		return (0);
	i = 0;
	result = 0;
	s1_temp = (unsigned char *)s1;
	s2_temp = (unsigned char *)s2;
	while (i < n)
	{
		if (*(s1_temp + i) != *(s2_temp + i))
		{
			result = ((int)*(s1_temp + i) - *(s2_temp + i));
			break ;
		}
		i += 1;
	}
	return (result);
}
