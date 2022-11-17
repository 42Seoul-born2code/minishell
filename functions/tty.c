#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd[2];
	pid_t	pid;


    pipe(fd);
	pid = fork();
	if (pid > 0)
	{
		int		r;
		char	buf[32];
		close(fd[1]);
		while ((r = read(fd[0], buf, sizeof(buf))) > 0)
		{
			write(1, buf, r);
		}
	}
	else
	{
		printf("%d\n", isatty(STDOUT_FILENO));
		dup2(fd[1], STDOUT_FILENO);
		dup2(fd[1], STDERR_FILENO);
		printf("%d\n", isatty(STDOUT_FILENO));
		printf("%d\n", isatty(STDERR_FILENO));
	}
	return (0);
}