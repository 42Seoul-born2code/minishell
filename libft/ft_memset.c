/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 13:50:59 by joonhan           #+#    #+#             */
/*   Updated: 2022/03/30 15:41:35 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *ptr, int c, size_t len)
{
	size_t			i;
	unsigned char	*temp;

	i = 0;
	temp = (unsigned char *)ptr;
	while (i < len)
	{
		temp[i] = (unsigned char)c;
		i += 1;
	}
	return (temp);
}
