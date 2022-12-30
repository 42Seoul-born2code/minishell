#include "mini_signal.h"

static void	handle_sigint_change(int signum)
{
	if (signum != SIGINT)
		return ;
	write(1, "\n", 1);
}

/* do nothing */
static void	handle_sigquit_change(int signum)
{
	if (signum == SIGQUIT)
	{
		ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
		return ;
	}
}

void	change_signal(void)
{
	signal(SIGINT, handle_sigint_change);
	signal(SIGQUIT, handle_sigquit_change);
}