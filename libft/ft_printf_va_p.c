/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_va_p.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 10:27:28 by joonhan           #+#    #+#             */
/*   Updated: 2022/09/17 12:34:18 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	print_recursive_hex(size_t addr, int result)
{
	char	*hex;

	hex = "0123456789abcdef";
	if (addr == 0)
		return (result);
	result = print_recursive_hex(addr / 16, result);
	return (result + write(1, &hex[addr % 16], 1));
}

int	ft_printf_va_p(void *p)
{
	int		result;
	size_t	addr;

	result = 0;
	addr = *(size_t *)&p;
	result += write(1, "0x", 2);
	if (p == NULL)
		return (result + write(1, "0", 1));
	else
		return (print_recursive_hex(addr, result));
}
