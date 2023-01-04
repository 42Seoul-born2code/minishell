#include "minishell.h"
#include "execute.h"
#include "utils.h"
#include <termios.h>
/*
	main ()				- Entry function of minishell

	argc				- The number of arguments
	argv				- The actual arguments
	envp				- The environment variables when executing minishell
*/
int	g_exit_code = EXIT_SUCCESS;

int	main(int argc, char **argv, char **envp)
{
	struct termios  term;

	tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ECHOCTL);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
	t_env_list	env_list;

	(void)argv;
	if (argc != 1)
		return (print_error(INVALID_ARGUMENT, argv[1]));
	save_envp_in_env_list(envp, &env_list);
	execute_minishell(&env_list);
	return (0);
}
