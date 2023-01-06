/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:17:40 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:17:41 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execute.h"
#include "utils.h"

/*
	init_token_list()	- Initialize token list

	token_list			- Entry point of linked list
*/
static t_token	*init_token_list(void)
{
	t_token	*token_list;

	token_list = malloc(sizeof(t_token));
	token_list->head_node = NULL;
	return (token_list);
}

/*
	main ()				- Entry function of minishell

	argc				- The number of arguments
	argv				- The actual arguments
	envp				- The environment variables when executing minishell
*/
int	g_exit_code = EXIT_SUCCESS;

int	main(int argc, char **argv, char **envp)
{
	t_token		*token_list;
	t_env_list	env_list;

	(void)argv;
	if (argc != 1)
		return (print_error(INVALID_ARGUMENT, argv[1]));
	token_list = init_token_list();
	save_envp_in_env_list(envp, &env_list);
	execute_minishell(token_list, &env_list);
	return (0);
}
