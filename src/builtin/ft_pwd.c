/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:19:55 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:20:02 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

// argv 2번째부터 NULL이 아니고, -로 시작하는 경우 EXIT_FAILURE반환
int	ft_pwd(char **argv, t_env_list *env_list)
{
	char	*cwd;

	if (*(argv + 1) && check_option(*(argv + 1)) == EXIT_FAILURE)
	{
		print_error(SYNTAX_ERROR, "\n");
		return (EXIT_FAILURE);
	}
	cwd = get_env_value(env_list, "PWD");
	if (!cwd)
	{
		print_error("Error: There is no path\n", NULL);
		return (EXIT_FAILURE);
	}
	printf("%s\n", cwd);
	return (EXIT_SUCCESS);
}
