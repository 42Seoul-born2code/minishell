/*

echo 'hello "world"' | cat > a

token_list

[echo]  ['hello "world"']   [|]      [cat]     [>]      [a]
word       word             pipe     word     rd     word

cmd         arg        			pipe    cmd   redirect   arg

<
>
<<
>>
|

if (node->next->type == PIPE)
if (node->next->type == REDIRECTION_LEFT)
if (node->next->type == REDIRECTION_RIGHT)
if (node->next->type == REDIRECTION_HEREDOC)
if (node->next->type == REDIRECTION_APPEND)


*/
/*

1. 맨 첫번째 token이 operator인경우 어떻게 처리?
-> 끝까지 다 돌면서 파싱을 해줄건지

정상적인 경우
1. FLAG(CMD 선언을 했다->CMD_MODE = 1) token이 일단 WORD이면 CMD
2. CMD 뒤에는 다음 operator가 올때까지 계속 WORD가 들어오면 모두 ARG 처리
    2-1. operator 가 > >> , <ㅣㄴ <<
3. operator를 만나면 operator로 저장 후, 1번으로 다시 돌아가서 똑같은 함수로 처리?

=============================
생각해보니 정상인 경우

첫번쨰 operator가 < or << 인경우는 정상임

===============================

# REDIECTION INPUT (<)

1. COMMAND 가 뒤에 오는 경우
[< FILE_NAME COMMAND ARGUMENTS]

OK : < Makefile cat 
OK : < Makefile ls -la 
OK : cat < Makefile

NO : < Makefile cat ls
NO : < Makefile cat ls -al


2. COMMAND 가 앞에 오는 경우

[COMMAND < FILE_NAME ]

OK : wc -l < Makefile

===============================

# REDIRECTION RIGHT, APPEND (>, >>)

[COMMAND ARGUMENT > FILE_NAME]

- OK : ls > outfile

[< FILE_NAME COMMAND ARGUMENT]

- OK : < infile cat Makefile

[COMMAND ARGUMENT >> FILE_NAME]

- OK : ls -la >> outfile

[>> FILE_NAME COMMAND ARGUMENT]

- OK : >> outfile cat Makefile

================================

[<< LIMITER COMMAND ARGUMENT]

- OK : << eof ls
- OK : << eof ls -al
- OK : << eof ls > outfile
- OK : << eof ls | << foe cat
- NO : << eof ls cat
- NO : << eof ls -la abcd

================================

[ PIPE ]

- OK : ls | >> outfile
- OK : ls | > outfile

*/

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
	t_token_node	*curr_content;
	t_parse_info	parse_info;

	init_parse_info(&parse_info);
	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		curr_content = (t_token_node *)curr_node->content;
		// 1. WORD 인 경우
		if (curr_content->type == WORD)
		{
			// 1-1. redirection 뒤는 무조건 파일 이름이 온다.
			// CASE1. ls > outfile
			// CASE2. > outfile ls
			// CASE3. ls >> outfile
			// CASE4. >> outfile ls
			// CASE5. < infile cat
			// CASE6. cat < infile
			if (parse_info.is_redirection_found == TRUE)
			{
				curr_content->type = FILE_NAME;
				parse_info.is_redirection_found = FALSE;
			}
			// 1-2. command 를 처음 발견한 경우
			// command 는 redirection 보다 우선순위가 낮음
			// CASE1. ls << eof 
			else if (parse_info.is_command_found == FALSE && parse_info.is_heredoc_found == FALSE)
			{
				curr_content->type = COMMAND;
				parse_info.is_command_found = TRUE;
			}
			// 1-3. HEREDOC 뒤에 오는 WORD 는 LIMITER 
			else if (parse_info.is_heredoc_found == TRUE)
			{
				curr_content->type = LIMITER;
				parse_info.is_heredoc_found = FALSE;
			}
			// 1-4. 그 외의 경우에는 모두 ARGUMENT 
			else
			{
				curr_content->type = ARGUMENT;
			}
		}

		// 2. <, >, >> 을 받았을 때
		else if ((curr_content->type == REDIR_LEFT || curr_content->type == REDIR_RIGHT || curr_content->type == REDIR_APPEND) \
		&& parse_info.is_redirection_found == FALSE)
		{
			parse_info.is_redirection_found = TRUE;
		}

		// 3. << HEREDOC을 받았을 때
		// CASE1. ls << eof cat
		// CASE2. ls cat << eof
		else if (curr_content->type == REDIR_HEREDOC)
		{
			parse_info.is_heredoc_found = TRUE;
		}

		// 4. 파이프를 받았을 때, 모두 False로 초기화
		else if (curr_content->type == PIPE)
		{
			parse_info.is_command_found = FALSE;
			parse_info.is_redirection_found = FALSE;
			parse_info.is_heredoc_found = FALSE;
		}
		printf("type: %d, word: %s\n", curr_content->type, curr_content->word);
		curr_node = curr_node->next;
	}
}

/*

	COMMAND << LIMITER COMMAND
	COMMAND COMMAND << LIMITER

	WORD[COMMAND] <<[HEREDOC] WORD[LIMITER] WORD[ARGUMENT] 
	ls << eof cat(ARGUMENT)
	ls cat(ARGUMENT) << eof

	So, before pipe, a word after command is considered arguments.

*/