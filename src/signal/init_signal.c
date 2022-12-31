#include "mini_signal.h"

int	exit_code;

static void	handle_sigint(int signum)
{
	if (signum != SIGINT)
		return ;
	printf("\n");
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
	exit_code = 1;
}

/* SIGINT : ctrl + c, SIGQUIT : ctrl + \ */
void	init_signal(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
