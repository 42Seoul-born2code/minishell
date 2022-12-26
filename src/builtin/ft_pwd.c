#include "builtin.h"


int ft_pwd(char **argv)
{
	char	*cwd;

	// arg 2번째부터 NULL이 아니고, -로 시작하는 경우 EXIT_FAILURE반환
	if (*(argv + 1) && check_option(*(argv + 1)) == EXIT_FAILURE)
	{
		printf("NO OPTION\n");
		return (EXIT_FAILURE);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		printf("ERROR\n");
		return (EXIT_FAILURE);
	}
	printf("%s\n",cwd);
	free(cwd);
	return (EXIT_SUCCESS);
}