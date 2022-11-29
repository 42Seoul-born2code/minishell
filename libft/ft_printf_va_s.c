/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_va_s.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 10:27:28 by joonhan           #+#    #+#             */
/*   Updated: 2022/09/17 12:23:36 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printf_va_s(char *s)
{
	size_t	len;

	len = 0;
	if (s == NULL)
		return (write(1, "(null)", 6));
	len = ft_strlen(s);
	return (write(1, s, len));
}
