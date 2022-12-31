#ifndef MINI_SIGNAL_H
# define MINI_SIGNAL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>

void	init_signal(void);
void	change_signal(void);
void	change_heredoc_signal(void);

#endif
