#include "minishell.h"
#include "mini_signal.h"

static void	handle_sigint_change(int signum)
{
	if (signum != SIGINT)
		return ;
	g_exit_code = 1;
	printf("\n");
}

void	change_heredoc_signal(void)
{
	signal(SIGINT, handle_sigint_change);
}