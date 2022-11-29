/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 14:40:39 by joonhan           #+#    #+#             */
/*   Updated: 2022/04/11 13:55:00 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_sign(int n)
{
	if (n >= 0)
		return (1);
	else
		return (-1);
}

static int	get_digits(int n)
{
	int	i;

	i = 1;
	while (n / 10 != 0)
	{
		i *= 10;
		n /= 10;
	}
	return (i);
}

void	ft_putnbr_fd(int n, int fd)
{
	int	sign;
	int	digits;
	int	divisor;

	sign = check_sign(n);
	digits = get_digits(n);
	if (sign == -1)
		write(fd, "-", 1);
	while (n > 0 || n < 0)
	{
		divisor = ((n / digits) * sign) + '0';
		write(fd, &divisor, 1);
		n %= digits;
		digits /= 10;
	}
	while (digits > 0)
	{
		write(fd, "0", 1);
		digits /= 10;
	}
}
