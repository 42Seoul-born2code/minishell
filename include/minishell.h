#ifndef MINISHELL_H
# define MINISHELL_H

/*	CUSTOM HEADERS		*/
# include "libft.h"
# include "tokenize.h"
# include "execute.h"
# include "parsing.h"
# include "utils.h"

/*	BUILTIN HEADERS		*/
# include <signal.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

# define ERROR	-1

# define INVALID_ARGUMENT	"❌Error: Invalid arguments"

extern int	g_exit_code;

#endif