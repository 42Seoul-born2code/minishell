/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_minishell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongkpa <jeongkpa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:17:42 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/08 16:01:15 by jeongkpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

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

static void	free_line_and_list(char *line, t_token *token_list)
{
	free(line);
	free_list_nodes(token_list);
}

static void	process_line(char *line, t_token *token_list, t_env_list *env_list)
{
	if (is_all_whitespace(line) == FALSE)
	{
		if (tokenize_line(line, token_list) == EXIT_SUCCESS)
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
	}
}

/*
	execute_minishell()	- Execute command until signaled by SIGINT or EOF.

	env_list			- Enviornment variable linked list at executing minishell
	
	curr_list			- Current token list node
	next_list			- Next token list node
	curr_node			- Current actual token node
*/
void	execute_minishell(t_token *token_list, t_env_list *env_list)
{
	char	*line;

	while (TRUE)
	{
		init_signal();
		echoctl_off();
		line = readline(PROMPT);
		if (line == NULL)
		{
			printf("exit\n");
			break ;
		}
		process_line(line, token_list, env_list);
		free_line_and_list(line, token_list);
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