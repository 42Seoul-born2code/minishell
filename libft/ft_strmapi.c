/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 12:53:34 by joonhan           #+#    #+#             */
/*   Updated: 2022/03/29 13:08:42 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	size_t			size;
	char			*ptr;

	i = 0;
	if (!s || !f)
		return (NULL);
	size = ft_strlen(s);
	ptr = (char *)malloc(sizeof(char) * (size + 1));
	if (ptr == NULL)
		return (NULL);
	while (*(s + i) != '\0')
	{
		*(ptr + i) = f(i, *(s + i));
		i += 1;
	}
	*(ptr + i) = '\0';
	return (ptr);
}
