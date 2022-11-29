#include "minishell.h"

/*
** main ()				- Entry Function of Minishell
**
** return				- Status Code of Exiting Minishell
** argc					- The Number of Argument
** argv					- The Actual Arguments
** envp					- The Environment Variables on the Entry Time
** input				- Variable for a User Input
** chunks				- Variable for Tokens of User Input
** syntax				- Variable for a Syntax Tree from Chunks
** envmap				- Variable for Maps the Environment Variables
*/

int	main(int argc, char **argv, char **envp)
{
	// char		*input;
	char		*prompt;
	t_env_list	curr_env;

	(void)envp;
	if (argc != 1)
		return (EXIT_FAILURE);
	prompt = "./minishell$ ";
	save_envp_in_list(envp, &curr_env);
	(void)argv;
	// while (TRUE)
	// {
	// 	input = readline(prompt);
	// 	if (!input)
	// 		break;
	// 	add_history(input);
	// 	free(input);
	// }
	// system("leaks minishell");
	return (0);
}