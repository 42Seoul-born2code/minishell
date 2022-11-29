/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 09:56:13 by joonhan           #+#    #+#             */
/*   Updated: 2022/03/30 15:13:03 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_size(int n)
{
	int	i;

	i = 1;
	while (n / 10 != 0)
	{
		i += 1;
		n /= 10;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*ptr;
	int		sign;
	int		size;

	if (n == 0)
		return (ft_strdup("0"));
	sign = 1;
	if (n < 0)
		sign = -1;
	size = get_size(n);
	if (n < 0)
		size += 1;
	ptr = (char *)ft_calloc(size + 1, sizeof(char));
	if (ptr == NULL)
		return (NULL);
	while (n != 0 && size > 0)
	{
		*(ptr + size - 1) = ((n % 10) * sign) + '0';
		n /= 10;
		size -= 1;
	}
	if (sign < 0)
		*(ptr + size - 1) = '-';
	return (ptr);
}
