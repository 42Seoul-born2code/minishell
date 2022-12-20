#include "execute.h"

void	print_token_word(t_token *token_list)
{
	t_list			*curr_node;
	t_token_node	*curr_token;

	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		printf("curr_token->word: %s\n", curr_token->word);
		curr_node = curr_node->next;
	}
}

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
	char	*line;
	t_token	*token_list;

	(void)env;
	token_list = malloc(sizeof(t_token));
	token_list->head_node = NULL;
	while (TRUE)
	{
		line = readline(PROMPT);
		if (!line)
			break ;
		tokenize_line(line, token_list);
		parsing(token_list);
		syntax_analysis(token_list);
		expansion(token_list);
		quote_removal(token_list);
		print_token_word(token_list);
		set_redirection(token_list);
		free_list_nodes(token_list);
		add_history(line);
		free(line);
	}
}

// [cat] [Makefile]
// execve(cmd_path, cmd_argv, envp);

// cat Makefile > outfile
