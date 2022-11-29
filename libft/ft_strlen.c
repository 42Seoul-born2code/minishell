/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@studnet.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 20:33:18 by joonhan           #+#    #+#             */
/*   Updated: 2022/08/24 16:45:09 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

ssize_t	ft_strlen(char const *str)
{
	ssize_t	len;

	len = 0;
	if (str == NULL)
		return (0);
	while (str[len] != '\0')
		len += 1;
	return (len);
}
