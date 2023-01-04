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
		return (print_error(INVALID_ARGUMENT, argv[1]));
	save_envp_in_env_list(envp, &env_list);
	execute_minishell(&env_list);
	return (0);
}
