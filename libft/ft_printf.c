/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 10:26:20 by joonhan           #+#    #+#             */
/*   Updated: 2022/09/17 12:47:49 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_printf_args(char format, va_list ap)
{
	if (format == 'c')
		return (ft_printf_va_c((char)va_arg(ap, int)));
	else if (format == 'd' || format == 'i')
		return (ft_printf_va_d((int)va_arg(ap, int)));
	else if (format == 'p')
		return (ft_printf_va_p((void *)va_arg(ap, void *)));
	else if (format == 's')
		return (ft_printf_va_s((char *)va_arg(ap, int *)));
	else if (format == 'u')
		return (ft_printf_va_u((unsigned int)va_arg(ap, int)));
	else if (format == 'x')
		return (ft_printf_va_x((unsigned int)va_arg(ap, int), 'x'));
	else if (format == 'X')
		return (ft_printf_va_x((unsigned int)va_arg(ap, int), 'X'));
	else if (format == '%')
		return (write(1, "%", 1));
	else
		return (0);
}

int	ft_printf(const char *str, ...)
{
	size_t	idx;
	size_t	str_len;
	int		ret_len;
	va_list	ap;

	idx = 0;
	str_len = ft_strlen(str);
	ret_len = 0;
	va_start(ap, str);
	while (idx < str_len && str[idx] != '\0')
	{
		if ((str[idx] == '%') && ft_strchr("cspdiuxX%", str[idx + 1]))
		{
			ret_len += ft_printf_args(str[idx + 1], ap);
			idx += 2;
		}
		else
		{
			write(1, &str[idx++], 1);
			ret_len += 1;
		}
	}
	va_end(ap);
	return (ret_len);
}
