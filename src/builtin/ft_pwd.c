#include "builtin.h"

// argv 2번째부터 NULL이 아니고, -로 시작하는 경우 EXIT_FAILURE반환
int	ft_pwd(char **argv, t_env_list *env_list)
{
	char	*cwd;

	if (*(argv + 1) && check_option(*(argv + 1)) == EXIT_FAILURE)
	{
		printf("%s\n", SYNTAX_ERROR);
		return (EXIT_FAILURE);
	}
	cwd = get_env_value(env_list, "PWD");
	if (!cwd)
	{
		printf("Error: There is no path\n");
		return (EXIT_FAILURE);
	}
	printf("%s\n", cwd);
	return (EXIT_SUCCESS);
}
