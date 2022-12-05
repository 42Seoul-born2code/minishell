#ifndef EXECUTE_H
# define EXECUTE_H

# include <readline/readline.h>
# include <readline/history.h>

# include "tokenize.h"
# include "parsing.h"
# include "utils.h"

void	execute_minishell(t_env_list env);

#endif