#include "execute.h"

void	free_list_nodes(t_token *lst)
{
	t_list				*curr_list;
	t_list				*next_list;
	t_token_node	*curr_node;

	curr_list = lst->head_node;
	while (curr_list != NULL)
	{
		next_list = curr_list->next;
		curr_node = curr_list->content;
		free(curr_node->word);
		free(curr_list);
		curr_list = next_list;
	}
	lst->head_node = NULL;
}

void	execute_minishell(t_env_list env)
{
	char		*line;
	char		*prompt;
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
		syntax_analysis(token_list);
		expansion(token_list);
		free_list_nodes(token_list);
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