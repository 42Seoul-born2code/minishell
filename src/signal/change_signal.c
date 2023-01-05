#include "mini_signal.h"

static void	handle_sigint_change(int signum)
{
	if (signum != SIGINT)
		return ;
	printf("\n");
}

/* do nothing */
static void	handle_sigquit_change(int signum)
{
	if (signum == SIGQUIT)
	{
		printf("Quit: 3\n");
		return ;
	}
}

void	change_signal(void)
{
	signal(SIGINT, handle_sigint_change);
	signal(SIGQUIT, handle_sigquit_change);
}
