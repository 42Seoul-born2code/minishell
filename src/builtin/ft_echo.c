/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:19:46 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:19:48 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

t_bool	is_valid_option(char *str)
{
	int	idx;

	if (str == NULL)
		return (FALSE);
	if (str[0] != '-')
		return (FALSE);
	if (str[1] == '\0')
		return (FALSE);
	idx = 1;
	while (str[idx] != '\0')
	{
		if (str[idx] != 'n')
			return (FALSE);
		idx += 1;
	}
	return (TRUE);
}

char	*save_result(char **argv, int idx, char *prev_word)
{
	char	*buffer;
	char	*result;

	buffer = NULL;
	result = NULL;
	buffer = ft_strdup(prev_word);
	if (prev_word != NULL)
		free(prev_word);
	result = ft_strjoin(buffer, argv[idx]);
	free(buffer);
	if (argv[idx + 1] != NULL)
	{
		buffer = ft_strdup(result);
		free(result);
		result = ft_strjoin(buffer, " ");
		free(buffer);
	}
	return (result);
}

char	*merge_argv(char **argv, \
					t_bool is_first_option_valid, t_bool *is_option_on)
{
	int		idx;
	char	*result;
	t_bool	is_option_over;

	idx = 1;
	result = NULL;
	is_option_over = FALSE;
	while (argv[idx] != NULL)
	{
		if (is_first_option_valid == TRUE && \
		is_valid_option(argv[idx]) == TRUE && is_option_over == FALSE)
		{
			*is_option_on = TRUE;
			idx += 1;
			continue ;
		}
		is_option_over = TRUE;
		result = save_result(argv, idx, result);
		idx += 1;
	}
	return (result);
}

int	ft_echo(char **argv)
{
	char	*result;
	t_bool	is_option_on;
	t_bool	is_first_option_valid;

	is_option_on = FALSE;
	is_first_option_valid = is_valid_option(argv[1]);
	result = merge_argv(argv, is_first_option_valid, &is_option_on);
	if (result == NULL)
	{
		if (is_option_on == FALSE)
			printf("\n");
	}
	else
	{
		printf("%s", result);
		if (is_option_on == FALSE)
			printf("\n");
	}
	free(result);
	return (EXIT_SUCCESS);
}

/*

	TEST CASES

	echo
	>

	echo -
	> -

	echo -n
	>

	echo -nnnn
	>

	echo -nnnn -n
	>
	
	echo -nnnn -n hello -n
	> hello -n$

	echo -n hello
	> hello$

	echo -nnnnnnnnnn hello
	> hello$

	echo -n -asd hello
	> -asd hello$
	
	echo -asdf -n hello
	> -asdf -n hello

	echo -n -nn hello
	> hello$

*/

/*

	CHESEO's advice

	1. builtin function 실행 후 exit status

	2. pwd : cd 를 하고나서 pwd 를 업데이트가 되어야 한다.

	3. cd 
		- cd 실패 시 어떻게 되어야 하는건지? (에러 메세지 출력)
		- chdir 상대 경로 또는 절대 경로로 주어야 함
		- 바로 반영이 안될 수 있다.

*/