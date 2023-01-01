#include "utils.h"

int	print_error(char *msg, char *arg)
{
	char	*error_msg;

	error_msg = ft_strjoin(msg, arg);
	ft_putstr_fd(error_msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	free(error_msg);
	return (EXIT_ERROR);
}
