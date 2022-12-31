#include "minishell.h"

/*
** main ()				- Entry Function of Minishell
**
** return				- Status Code of Exiting Minishell
** argc					- The Number of Argument
** argv					- The Actual Arguments
** envp					- The Environment Variables on the Entry Time
*/

int	g_exit_code = EXIT_SUCCESS;

int	main(int argc, char **argv, char **envp)
{
	t_env_list	env_list;

	(void)argv;
	if (argc != 1)
		return (EXIT_FAILURE);
	save_envp_in_env_list(envp, &env_list);
	execute_minishell(&env_list);
	// system("leaks minishell");
	return (0);
}


/*

	터지는 케이스

	리다이렉션 하나만 입력 되었을 때
	< , <<
	- segfault

	<<<
	- heredoc 이 실행됨

	lss
	- 미니쉘이 바로 꺼짐

*/