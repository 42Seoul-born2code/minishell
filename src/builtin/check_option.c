#include "builtin.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <fcntl.h>

# include "tokenize.h"
# include "parsing.h"
# include "expansion.h"
# include "utils.h"

/*
	option O -> exit_code = EXIT_FAILURE
	option X -> exit_code = EXIT_SUCCESS
*/
int	check_option(char *argv)
{
	int	exit_code;

	exit_code = EXIT_SUCCESS;
	if (*argv == '-')
		exit_code = EXIT_FAILURE;
	return (exit_code);
}
