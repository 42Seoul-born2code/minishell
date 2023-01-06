#include "execute.h"

void	save_origin_fd(int origin_fd[2])
{
	origin_fd[0] = dup(STDIN_FILENO);
	origin_fd[1] = dup(STDOUT_FILENO);
}

void	rollback_origin_fd(int origin_fd[2])
{
	dup2(origin_fd[0], STDIN_FILENO);
	dup2(origin_fd[1], STDOUT_FILENO);
	close(origin_fd[0]);
	close(origin_fd[1]);
}
