#include "execute.h"

/*
	토큰화 확인용 함수
*/
void	print_token_word(t_token *token_list)
{
	t_list			*curr_node;
	t_token_node	*curr_token;

	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		printf("curr_token->word: %s, type: %d\n", curr_token->word, curr_token->type);
		curr_node = curr_node->next;
	}
}

static void	free_list_nodes(t_token *lst)
{
	t_list			*curr_list;
	t_list			*next_list;
	t_token_node	*curr_node;

	curr_list = lst->head_node;
	while (curr_list != NULL)
	{
		next_list = curr_list->next;
		curr_node = curr_list->content;
		free(curr_node->word);
		free(curr_node);
		free(curr_list);
		curr_list = next_list;
	}
	lst->head_node = NULL;
}

static t_bool	is_all_whitespace(char *line)
{
	int i;
	t_bool all_whitespace;

	i = 0;
	all_whitespace = TRUE;
	while (line[i] != '\0')
	{
		if (is_whitespace(line[i]) == FALSE)
			all_whitespace = FALSE;
		return all_whitespace;
		i += 1;
	}
	return all_whitespace;
}

void	execute_minishell(t_env_list *env_list)
{
	char	*line;
	t_token	*token_list;

	(void)env_list;
	token_list = malloc(sizeof(t_token));
	token_list->head_node = NULL;
	while (TRUE)
	{
		init_signal();
		line = readline(PROMPT);
		if (line == NULL)
			break ;
		if (line[0] != '\0' && is_all_whitespace(line) == FALSE)
		{
			if (tokenize_line(line, token_list) == EXIT_ERROR)
			{
				free(line);
				free_list_nodes(token_list);
				continue ;
			}
			parsing(token_list);
			if (syntax_analysis(token_list) == EXIT_ERROR)
			{
				free(line);
				free_list_nodes(token_list);
				continue ;
			}
			expansion(token_list, env_list);
			quote_removal(token_list);
			// print_token_word(token_list);
			execute_command(token_list, env_list);
			free_list_nodes(token_list);
			add_history(line);
		}
		free(line);
		// system("leaks minishell");
	}
}
