#include <stdio.h>
#include "builtin.h"
#include "libft.h"

t_bool	is_valid_option(char *str)
{
	int	idx;

	if (str == NULL)
		return (FALSE);
	if (str[0] != '-')
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

void	ft_echo(char **argv)
{
	int		idx;
	char	*buffer;
	char	*result;
	t_bool	is_option_on;
	t_bool	is_first_option_valid;

	idx = 1;
	buffer = NULL;
	result = NULL;
	is_option_on = FALSE;
	is_first_option_valid = is_valid_option(argv[idx]);
	while (argv[idx] != NULL)
	{
		if (is_first_option_valid == TRUE && is_valid_option(argv[idx]) == TRUE)
		{
			is_option_on = TRUE;
			idx += 1;
		}
		else
		{
			buffer = ft_strdup(result);
			if (result != NULL)
				free(result);
			result = ft_strjoin(buffer, argv[idx]);
			if (argv[idx + 1] != NULL)
			{
				buffer = ft_strdup(result);
				free(result);
				result = ft_strjoin(buffer, " ");
			}
			idx += 1;
		}
	}
	if (result == NULL)
		printf("\n");
	else
	{
		printf("%s", result);
		if (is_option_on == FALSE)
			printf("\n");
	}
}

/*

	TEST CASES

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