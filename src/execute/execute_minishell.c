#include "execute.h"

void	execute_minishell(t_env_list env)
{
	char	*line;
	char	*prompt;
	t_token	*token_list;

	(void)env;
	prompt = "./minishell$ ";
	token_list = malloc(sizeof(t_token));
	token_list->head_node = NULL;
	while (TRUE)
	{
		line = readline(prompt);
		if (!line)
			break ;
		tokenize_line(line, token_list);
		parsing(token_list);
		// TODO 문법 검사
		add_history(line);
		free(line);
	}
}

/*

	echo "hello 'world'"
	echo "hello \"world\""
	cat < main.c
	ls > output
	ls -l | grep main
	ls -l | grep main >> output
	<< HELLO cat | wc -l >> outfile

*/

/*
	3. Shell commands
	
	echo hello world
	[echo, hello, world]
	[word, word, word]
	[file, arguments, arguments]

	echo -n hello world
	execve("filename", argv, envp )
	-
*/