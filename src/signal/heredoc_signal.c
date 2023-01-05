#include "minishell.h"
#include "mini_signal.h"

static void	handle_sigint_change_c(int signum)
{
	if (signum != SIGINT)
		return ;
	exit(1);
}

void	change_heredoc_signal_child(void)
{
	signal(SIGINT, handle_sigint_change_c);
}

static void	handle_sigint_change_p(int signum)
{
	if (signum != SIGINT)
		return ;
	printf("\n");
}

void	change_heredoc_signal_parent(void)
{
	signal(SIGINT, handle_sigint_change_p);
}