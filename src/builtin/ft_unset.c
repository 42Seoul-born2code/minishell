/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:19:56 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:20:32 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/*
	option O -> exit_code = EXIT_FAILURE
	option X -> exit_code = EXIT_SUCCESS
*/
static int	is_option_included(char *argv)
{
	if (check_option(argv) == EXIT_FAILURE)
	{
		print_error(INVALID_OPTION, argv);
		return (TRUE);
	}
	return (FALSE);
}

/*
	Invalid env_name -> exit_code = EXIT_FAILURE
	Invalid option -> exit_code = 2
*/
static void	remove_env(t_list *target, t_env_list *env_list)
{
	t_list		*prev_node;
	t_list		*curr_node;

	prev_node = NULL;
	curr_node = env_list->head_node;
	if (curr_node == target)
		env_list->head_node = target->next;
	else
	{
		while (curr_node != NULL)
		{
			if (curr_node == target)
				break ;
			prev_node = curr_node;
			curr_node = curr_node->next;
		}
		prev_node->next = curr_node->next;
	}
	free(((t_env_node *)target->content)->key);
	if (((t_env_node *)target->content)->value != NULL)
		free(((t_env_node *)target->content)->value);
	free((t_env_node *)target->content);
	free(target);
}

static t_list	*get_env(char *key, t_env_list *env_list)
{
	t_list		*list_node;
	t_env_node	*env_node;

	list_node = env_list->head_node;
	while (list_node)
	{
		env_node = list_node->content;
		if (ft_strcmp(env_node->key, key) == 0)
			return (list_node);
		list_node = list_node->next;
	}
	return (NULL);
}

/*
	LINE76: 인자가 들어오지 않는 경우 return
	LINE80: 옵션이 있는 경우 syntax error 출력 후 return
	LINE82: 인자가 "=" 인경우 not_valid_identifier 오류 출력
	LINE84: 환경변수 이름 규칙에 맞지 않으면 not_valid_identifier 오류 출력
*/
int	ft_unset(char **argv, t_env_list *env_list)
{
	int		result;
	t_list	*target;

	result = EXIT_SUCCESS;
	if (*(argv + 1) == NULL)
		return (result);
	argv += 1;
	while (*argv)
	{
		if (is_option_included(*argv) == TRUE)
			return (2);
		else if (ft_strchr(*argv, '='))
			result = print_error(NOT_VALID_IDENTIFIER, *argv);
		else if (is_valid_variable_name(*argv) == FALSE)
			result = print_error(NOT_VALID_IDENTIFIER, *argv);
		else
		{
			target = get_env(*argv, env_list);
			if (target != NULL)
				remove_env(target, env_list);
		}
		argv++;
	}
	return (result);
}
