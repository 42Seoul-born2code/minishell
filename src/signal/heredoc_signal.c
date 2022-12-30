#include "mini_signal.h"

int	exit_code;

static void	handle_sigint_change(int signum)
{
	if (signum != SIGINT)
		return ;
	exit_code = 1;
	exit(exit_code);
}

void	change_heredoc_signal(void)
{
	signal(SIGINT, handle_sigint_change);
	signal(SIGQUIT, SIG_IGN);
}