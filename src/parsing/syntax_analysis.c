#include "parsing.h"

t_bool	is_continuous_pipe(t_list *curr_node)
{
	t_list			*next_node;
	t_token_node	*curr_token;
	t_token_node	*next_token;

	next_node = curr_node->next;
	curr_token = curr_node->content;
	next_token = next_node->content;
	if (curr_token->type == PIPE && next_token->type == PIPE)
		return (TRUE);
	return (FALSE);
}

t_bool	is_next_token_operator(t_list *curr_node)
{
	t_list			*next_node;
	t_token_node	*next_token;

	next_node = curr_node->next;
	next_token = next_node->content;
	if (is_operator(next_token->word) == TRUE)
		return (TRUE);
	return (FALSE);
}

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
			// 2. 파이프 바로 뒤에 파이프가 왔을 때
			else if (is_continuous_pipe(curr_node) == TRUE)
			{
				printf("파이프 뒤에 파이프네유? FAIL 드립니다.\n");
			}
			// 3. redirection 바로 뒤에 operator 가 왔는가?
			else if (curr_token->type != PIPE && \
					is_next_token_operator(curr_node) == TRUE)
			{
				printf("redirection 뒤에 operator 가 왔네유? FAIL 드립니다.\n");
			}
		}
		curr_node = curr_node->next;
	}
}
