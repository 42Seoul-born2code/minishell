/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 13:07:23 by joonhan           #+#    #+#             */
/*   Updated: 2022/03/30 15:14:59 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int				result;
	size_t			i;
	unsigned char	*s1_temp;
	unsigned char	*s2_temp;

	if (n == 0)
		return (0);
	result = 0;
	i = 0;
	s1_temp = (unsigned char *)s1;
	s2_temp = (unsigned char *)s2;
	while (*(s1_temp + i) != '\0' && *(s2_temp + i) != '\0')
	{
		if (i + 1 == n)
			break ;
		if (*(s1_temp + i) != *(s2_temp + i))
			break ;
		i += 1;
	}
	result = (int)(*(s1_temp + i) - *(s2_temp + i));
	return (result);
}
