#include "parsing.h"

static void	init_parse_info(t_parse_info *parse_info)
{
	parse_info->is_command_found = FALSE;
	parse_info->is_redirection_found = FALSE;
	parse_info->is_heredoc_found = FALSE;
}

void	parsing(t_token *token_list)
{
	t_list			*curr_node;
	t_token_node	*curr_token;
	t_parse_info	parse_info;

	init_parse_info(&parse_info);
	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		curr_token = (t_token_node *)curr_node->content;
		// 1. WORD 인 경우
		if (curr_token->type == WORD)
		{
			// 1-1. redirection 뒤는 무조건 파일 이름이 온다.
			if (parse_info.is_redirection_found == TRUE)
			{
				curr_token->type = FILE_NAME;
				parse_info.is_redirection_found = FALSE;
			}
			// 1-2. command 를 처음 발견한 경우
			// command 는 redirection 보다 우선순위가 낮음
			// CASE1. ls << eof 
			else if (parse_info.is_command_found == FALSE && parse_info.is_heredoc_found == FALSE)
			{
				curr_token->type = COMMAND;
				parse_info.is_command_found = TRUE;
			}
			// 1-3. HEREDOC 뒤에 오는 WORD 는 LIMITER 
			else if (parse_info.is_heredoc_found == TRUE)
			{
				curr_token->type = LIMITER;
				parse_info.is_heredoc_found = FALSE;
			}
			// 1-4. 그 외의 경우에는 모두 ARGUMENT 
			else
			{
				curr_token->type = ARGUMENT;
			}
		}

		// 2. <, >, >> 을 받았을 때
		else if ((curr_token->type == REDIR_LEFT || curr_token->type == REDIR_RIGHT || curr_token->type == REDIR_APPEND) \
		&& parse_info.is_redirection_found == FALSE)
		{
			parse_info.is_redirection_found = TRUE;
		}

		// 3. << HEREDOC을 받았을 때
		// CASE1. ls << eof cat
		// CASE2. ls cat << eof
		else if (curr_token->type == REDIR_HEREDOC)
		{
			parse_info.is_heredoc_found = TRUE;
		}

		// 4. 파이프를 받았을 때, 모두 False로 초기화
		else if (curr_token->type == PIPE)
		{
			parse_info.is_command_found = FALSE;
			parse_info.is_redirection_found = FALSE;
			parse_info.is_heredoc_found = FALSE;
		}
		printf("type: %d, word: %s\n", curr_token->type, curr_token->word);
		curr_node = curr_node->next;
	}
}
