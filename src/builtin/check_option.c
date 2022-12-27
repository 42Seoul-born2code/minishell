#include "builtin.h"

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
