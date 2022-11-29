/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_va_x.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 10:27:28 by joonhan           #+#    #+#             */
/*   Updated: 2022/09/17 12:47:55 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_printf_recursive_va_x(unsigned int n, char c, int result)
{
	char	*hex;

	if (c == 'x')
		hex = "0123456789abcdef";
	else
		hex = "0123456789ABCDEF";
	if (n == 0)
		return (result);
	result = ft_printf_recursive_va_x(n / 16, c, result);
	return (result + write(1, &hex[n % 16], 1));
}

int	ft_printf_va_x(unsigned int n, char c)
{
	int	result;

	result = 0;
	if (n == 0)
		return (write(1, "0", 1));
	return (ft_printf_recursive_va_x(n, c, result));
}
