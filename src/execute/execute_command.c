#include "execute.h"

static e_command_type	get_execute_type(t_token *token_list)
{
	t_list			*curr_node;
	t_token_node	*curr_token;

	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		if (curr_token->type == PIPE)
			return (MULTI_COMMAND);
		curr_node = curr_node->next;
	}
	return (SIMPLE_COMMAND);
}

void	execute_command(t_token *token_list, t_env_list env_list)
{
	// TODO
	// 1. 파이프가 존재하지 않는 simple command 는 fork 를 수행해서 실행한다.
	// 2. 파이프가 존재하는 multi command 는 파이프 생성 후 fork 를 실행한다.
	if (get_execute_type(token_list) == SIMPLE_COMMAND)
	{
		execute_simple_command(token_list, env_list);
	}
	else
	{
		// execute_multi_command(token_list, env_list);
	}
}
