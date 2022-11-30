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
	t_env_list	curr_env;

	if (argc != 1)
		return (EXIT_FAILURE);
	(void)argv;
	save_envp_in_list(envp, &curr_env);
	execute_minishell(curr_env);
	// system("leaks minishell");
	return (0);
}