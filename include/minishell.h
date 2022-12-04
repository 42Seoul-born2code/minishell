#ifndef MINISHELL_H
# define MINISHELL_H

/*	BUILTIN HEADERS		*/
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

/*	CUSTOM HEADERS		*/
# include "libft.h"
# include "tokenize.h"
# include "execute.h"
# include "utils.h"

# define ERROR	-1

# define INVALID_ARGUMENT	"❌Error: Invalid arguments"


#endif