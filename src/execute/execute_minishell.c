#include "execute.h"

static t_token	*init_token_list(void);
static t_bool	is_all_whitespace(char *line);
static void		free_list_nodes(t_token *lst);
void			execute_minishell(t_env_list *env_list);

// TODO: 전부 끝내고 삭제하기
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

/*
	init_token_list()	- Initialize token list

	token_list			- Entry point of linked list
*/
static t_token	*init_token_list(void)
{
	t_token	*token_list;

	token_list = malloc(sizeof(t_token));
	token_list->head_node = NULL;
	return (token_list);
}

/*
	is_all_whitespace()	- Check if user input is all whitespace
*/
static t_bool	is_all_whitespace(char *line)
{
	int	idx;

	idx = 0;
	while (line[idx] != '\0')
	{
		if (is_whitespace(line[idx]) == FALSE)
			return (FALSE);
		idx += 1;
	}
	return (TRUE);
}

/*
	free_list_nodes()	- Free all token list nodes and node's content

	lst					- Entry point of linked list
	
	curr_list			- Current token list node
	next_list			- Next token list node
	curr_node			- Current actual token node
*/
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

/*
	execute_minishell()	- Execute command until signaled by SIGINT or EOF.

	env_list			- Enviornment variable linked list at executing minishell
	
	curr_list			- Current token list node
	next_list			- Next token list node
	curr_node			- Current actual token node
*/
void	execute_minishell(t_env_list *env_list)
{
	char	*line;
	t_token	*token_list;

	token_list = init_token_list();
	while (TRUE)
	{
		init_signal();
		line = readline(PROMPT);
		if (line == NULL)
			break ;
		if (is_all_whitespace(line) == FALSE \
			&& tokenize_line(line, token_list) == EXIT_SUCCESS)
		{
			parsing(token_list);
			if (syntax_analysis(token_list) == SYNTAX_OK)
			{
				expansion(token_list, env_list);
				quote_removal(token_list);
				execute_command(token_list, env_list);
			}
		}
		add_history(line);
		free(line);
		free_list_nodes(token_list);
		// system("leaks minishell");// TODO: 통합 테스트 끝난 다음에 삭제하기 (noriminette 은 OK)
	}
}

/*

	// TODO: 멀티 파이프에서 맨 마지막 명령어 실행 결과가 반환해야함
	// 현재는 exit(1)로 반환받고 있음
	cat | cat | ls
	ctrl + c
	echo $?
	> 0

*/