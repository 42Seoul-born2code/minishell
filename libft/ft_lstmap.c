/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 14:05:36 by joonhan           #+#    #+#             */
/*   Updated: 2022/03/31 10:59:56 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*curr;
	t_list	*node;

	if (lst == NULL || f == NULL)
		return (NULL);
	new_lst = NULL;
	curr = lst;
	while (curr != NULL)
	{
		node = ft_lstnew(f(curr->content));
		if (node == NULL)
		{
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		ft_lstadd_back(&new_lst, node);
		node = node->next;
		curr = curr->next;
	}
	return (new_lst);
}
