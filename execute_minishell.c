#include "minishell.h"

void	execute_minishell(t_env_list env)
{
	char		*input;
	char		*prompt;

	(void)env;
	prompt = "./minishell$ ";
	while (TRUE)
	{
		input = readline(prompt);
		if (!input)
			break;
		add_history(input);
		free(input);
	}
}