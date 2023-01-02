#include "minishell.h"
#include "execute.h"
#include "utils.h"

/*
	main ()				- Entry function of minishell

	argc				- The number of arguments
	argv				- The actual arguments
	envp				- The environment variables when executing minishell
*/

int	g_exit_code = EXIT_SUCCESS;

int	main(int argc, char **argv, char **envp)
{
	t_env_list	env_list;

	(void)argv;
	if (argc != 1)
		return (EXIT_FAILURE);
	save_envp_in_env_list(envp, &env_list);
	// printf("1st env_list\n");
	// print_env_list(&env_list);
	execute_minishell(&env_list);
	return (0);
}
/*

	minishell 실행 후 minishell 실행 시 ctrl \ 누르면 Quit 3 출력되는 문제

*/