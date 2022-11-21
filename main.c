#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	*prompt;

	if (argc != 1)
		return (EXIT_FAILURE);
	(void)argv;
	(void)envp;
	prompt = "./minishell$ ";
	printf("HELLO MINISHELL!\n");
	while (TRUE)
	{
		input = readline(prompt);
		if (!input)
			break;
		add_history(input);
		free(input);
	}
	return (0);
}