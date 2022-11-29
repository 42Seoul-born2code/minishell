/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_va_u.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 10:27:28 by joonhan           #+#    #+#             */
/*   Updated: 2022/09/17 12:47:58 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_printf_recursive_va_u(unsigned int n, int result)
{
	char	c;

	if (n == 0)
		return (result);
	result = ft_printf_recursive_va_u(n / 10, result);
	c = (n % 10) + '0';
	return (result + write(1, &c, 1));
}

int	ft_printf_va_u(unsigned int n)
{
	int	result;

	result = 0;
	if (n == 0)
		return (write(1, "0", 1));
	return (ft_printf_recursive_va_u(n, result));
}
