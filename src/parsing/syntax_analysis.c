#include "parsing.h"

// 오퍼레이터가 연속으로 나온다면 TRUE, + 에러처리까지
// t_bool	is_continuos_operator(t_list *curr_node, t_token_node *curr_token)
// {
// 	e_meta	type;
// 	t_token_node	*next_token;

// 	type = curr_token->type;
// 	if (type == PIPE || type == REDIR_HEREDOC || type == REDIR_APPEND \
// 		|| type == REDIR_LEFT || type == REDIR_RIGHT)
// 	{
// 		if (curr_node->next != NULL && )
// 		return (TRUE);
// 	}
// 	else
// 		return (FALSE);
// }

// 1. operator 인가?
// 	1) next 가 NULL 인가? -> TRUE -> ERROR
	// cat |
//	2) next 가 operator 인가? -> TRUE -> ERROR
	// cat | >


void	syntax_analysis(t_token *token_list)
{
	t_list			*curr_node;
	t_token_node	*curr_token;

	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		if (is_operator(curr_token->word) == TRUE)
		{
			// 1. operator 다음에 아무 것도 없을 때
			if (curr_node->next == NULL)
			{
				printf("다음 노드가 없네유? FAIL 드립니다.\n");
			}
			// 2. operator 가 연속되었을 때
			// TODO 62번줄에 입력한 예외처리를 조건문으로 처리하기
			else if (TRUE)
			{
				printf("또 operator 네유? FAIL 드립니다.\n");
			}
		}
		// if (is_continuos_operator(curr_node, curr_token) != TRUE)
		// 	curr_node = curr_node->next;
		// else
		// 	throw_error();
		curr_node = curr_node->next;
	}
}
/*

	1. 따옴표가 제대로 닫혔는가? (DONE)
		- [[echo], ['hello "world"]] | cat

	2. operator 바로 뒤에 operator 인가?
		- 1) || or | | (파이프 뒤에 파이프)
		- 2) redirection 뒤에 operator 
		- 반례
			- OK: ls | > outfile
			- OK: ls | >> outfile

*/