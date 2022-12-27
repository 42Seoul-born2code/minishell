#include "builtin.h"

// argv 2번째부터 NULL이 아니고, -로 시작하는 경우 EXIT_FAILURE반환
int	ft_pwd(char **argv)
{
	char	*cwd;

	if (*(argv + 1) && check_option(*(argv + 1)) == EXIT_FAILURE)
	{
		printf("%s\n", SYNTAX_ERROR);
		return (EXIT_FAILURE);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		printf("Error: There is no path\n");
		return (EXIT_FAILURE);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (EXIT_SUCCESS);
}
