#ifndef EXECUTE_H
# define EXECUTE_H

# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>

# include "tokenize.h"
# include "parsing.h"
# include "expansion.h"
# include "utils.h"

# define PROMPT "./minishell$ "

void	execute_minishell(t_env_list env);
void	execute_command(t_token *token_list, t_env_list env_list);

#endif