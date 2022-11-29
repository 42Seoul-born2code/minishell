/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@studnet.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 19:47:18 by joonhan           #+#    #+#             */
/*   Updated: 2022/08/24 21:02:48 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*free_fd(t_node **p_head, int fd)
{
	t_node	*prev;
	t_node	*curr;

	prev = NULL;
	curr = *p_head;
	while (curr != NULL)
	{
		if (curr->fd == fd)
		{
			if (curr->backup != NULL)
				free(curr->backup);
			if (prev == NULL)
				*p_head = curr->next;
			else
				prev->next = curr->next;
			free(curr);
			break ;
		}
		prev = curr;
		curr = curr->next;
	}
	return (NULL);
}

t_node	*find_fd(t_node **p_head, int fd)
{
	t_node	*new;
	t_node	*curr;
	t_node	*prev;

	prev = NULL;
	curr = *p_head;
	while (curr != NULL)
	{
		if (curr->fd == fd)
			return (curr);
		prev = curr;
		curr = curr->next;
	}
	new = (t_node *)malloc(sizeof(t_node));
	if (new == NULL)
		return (NULL);
	new->backup = NULL;
	new->fd = fd;
	new->next = NULL;
	if (*p_head == NULL)
		*p_head = new;
	else
		prev->next = new;
	return (new);
}
