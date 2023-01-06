/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:19:36 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:19:48 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

t_bool	is_valid_variable_name(char *str)
{
	int	idx;

	if (ft_isdigit(str[0]) == TRUE)
		return (FALSE);
	idx = 0;
	while (str[idx] != '\0')
	{
		if (ft_isalnum(str[idx]) == TRUE || str[idx] == '_')
			idx += 1;
		else
			return (FALSE);
	}
	return (TRUE);
}

void	ft_lstadd_env_node(t_env_list *env_list, char *key, char *value)
{
	t_env_node	*new_node;

	new_node = malloc(sizeof(t_env_node));
	new_node->key = key;
	new_node->value = value;
	ft_lstadd_back(&env_list->head_node, ft_lstnew(new_node));
}

/*
	option O -> exit_code = EXIT_FAILURE
	option X -> exit_code = EXIT_SUCCESS
*/
int	check_option(char *argv)
{
	int	exit_code;

	exit_code = EXIT_SUCCESS;
	if (*argv == '-')
		exit_code = EXIT_FAILURE;
	return (exit_code);
}
