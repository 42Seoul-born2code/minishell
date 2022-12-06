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

- OK : < outfile cat Makefile

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

void	parsing(t_token *token_list)
{
	t_list				*curr_node;
	t_token_node	*curr_content;
	t_bool				is_command_found;

	is_command_found = FALSE;
	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		curr_content = (t_token_node *)curr_node->content;
		if (curr_content->type == WORD && is_command_found == FALSE)
		{
			curr_content->type = COMMAND;
			is_command_found = TRUE;
		}
		// 1. Redirection input (<)
		// - [COMMAND < FILE_NAME ]
		else if (is_command_found == TRUE && curr_content->type == REDIR_LEFT)
		{

		}
		// - [< FILE_NAME COMMAND ARGUMENTS]
		else if ()
		{
			
		}
		printf("type: %d, word: %s\n", curr_content->type, curr_content->word);
		curr_node = curr_node->next;
	}
}
