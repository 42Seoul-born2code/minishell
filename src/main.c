#include "minishell.h"

/*
** main ()				- Entry Function of Minishell
**
** return				- Status Code of Exiting Minishell
** argc					- The Number of Argument
** argv					- The Actual Arguments
** envp					- The Environment Variables on the Entry Time
*/

int	main(int argc, char **argv, char **envp)
{
	t_env_list	*env_list;

	(void)argv;
	if (argc != 1)
		return (EXIT_FAILURE);
	env_list = init_env_list(envp);
	execute_minishell(env_list);
	// system("leaks minishell");
	return (0);
}