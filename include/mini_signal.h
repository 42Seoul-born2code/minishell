#ifndef MINI_SIGNAL_H
# define MINI_SIGNAL_H

# include <signal.h>
# include <unistd.h>
# include <sys/ioctl.h>
# include <execute.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

void	init_signal(void);
// void	change_signal(void);
// void	heredoc_signal(void);
// void	multishell_signal(void);

#endif
