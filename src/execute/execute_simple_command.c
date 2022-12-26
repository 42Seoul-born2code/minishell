#include "execute.h"

void	execute_simple_command(t_token *token_list, t_env_list *env_list)
{
	if (is_command_builtin_function(token_list) == TRUE)
	{
		process_builtin_function(token_list, env_list, SIMPLE_COMMAND);
	}
	else
	{
		fork_process(token_list, env_list);
	}
}
