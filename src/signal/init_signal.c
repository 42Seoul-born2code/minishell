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

static void	handle_sigquit(int signum)
{
	if (signum != SIGQUIT)
		return ;
	printf("Quit: 3\n");
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
	exit_code = 131;
	// 128 + signal number
}

/* SIGINT : ctrl + c, SIGQUIT : ctrl + \ */
void	init_signal(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}

// void		handle_signal(int signo)
// {
// 	pid_t	pid;
// 	int		status;

// 	pid = waitpid(-1, &status, WNOHANG);
// 	if (signo == SIGINT)
// 	{
// 		if (pid == -1)
// 		{
// 			printf("\b\b  \b\b\n");
// 			printf("%s", PROMPT);
// 			exit_code = 1;
// 		}
// 		else
// 			printf("\n");
// 	}
// 	else if (signo == SIGQUIT)
// 	{
// 		if (pid == -1)
// 			printf("\b\b  \b\b");
// 		else
// 			printf("Quit: 3\n");
// 	}
// }

// void		init_signal(void)
// {
// 	signal(SIGINT, handle_signal);
// 	signal(SIGQUIT, handle_signal);
// }