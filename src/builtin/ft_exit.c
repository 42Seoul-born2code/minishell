/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:19:50 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:19:55 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"
#include "execute.h"

static int	check_lld_overflow(unsigned long long res, int sign)
{
	if (res > 9223372036854775807UL && sign > 0)
		return (FALSE);
	if (res > 9223372036854775808UL && sign < 0)
		return (FALSE);
	return (TRUE);
}

static int	valid_exit_code(const char *s)
{
	char				*ptr;
	unsigned long long	res;
	int					sign;

	ptr = (char *)s;
	res = 0;
	sign = 1;
	if (*ptr == '-')
	{
		sign = -1;
		ptr++;
	}
	while (*ptr)
	{
		if (res > res * 10 + (*ptr - '0'))
			return (FALSE);
		res = res * 10 + (*ptr - '0');
		ptr++;
	}
	return (check_lld_overflow(res, sign));
}

static unsigned char	check_arg(char *arg)
{
	char	*ptr;

	ptr = arg;
	if (*ptr == '-')
		ptr++;
	while (*ptr)
	{
		if (ft_isdigit(*ptr) == FALSE)
		{
			print_error(NOT_NUMERIC_ARGUMENT, arg);
			exit(255);
		}
		ptr++;
	}
	if (valid_exit_code(arg) == FALSE)
	{
		print_error(NOT_NUMERIC_ARGUMENT, arg);
		exit(255);
	}
	return (ft_atoi(arg));
}

static void	free_env_list(t_env_list *env_list)
{
	t_list			*curr_list;
	t_list			*next_list;
	t_env_node		*curr_node;

	curr_list = env_list->head_node;
	while (curr_list != NULL)
	{
		next_list = curr_list->next;
		curr_node = curr_list->content;
		free(curr_node->key);
		if (curr_node->value != NULL)
		{
			free(curr_node->value);
		}
		free(curr_node);
		free(curr_list);
		curr_list = next_list;
	}
	env_list->head_node = NULL;
}

int	ft_exit(char **argv, t_env_list *env_list, t_command_type command_type)
{
	char			*arg;
	unsigned char	exit_code;

	exit_code = EXIT_SUCCESS;
	if (command_type == SIMPLE_COMMAND)
		print_error("exit", NULL);
	if (*(argv + 1))
	{
		arg = ft_strtrim(*(argv + 1), " \n\t\f\v\r");
		exit_code = check_arg(arg);
		free(arg);
		if (*(argv + 2) != NULL)
		{
			return (print_error(TOO_MANY_ARGUMNET, NULL));
		}
	}
	free_env_list(env_list);
	exit(exit_code);
}
