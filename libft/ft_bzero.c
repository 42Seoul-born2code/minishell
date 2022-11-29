/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 13:23:46 by joonhan           #+#    #+#             */
/*   Updated: 2022/03/30 15:40:17 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *dest, size_t size)
{
	size_t			i;
	unsigned char	*temp;

	temp = (unsigned char *)dest;
	i = 0;
	while (i < size)
	{
		*(temp + i) = 0;
		i += 1;
	}
}
