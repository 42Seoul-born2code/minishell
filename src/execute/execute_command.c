#include "execute.h"

static t_command_type	get_execute_type(t_token *token_list)
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

void	check_heredoc(t_token *token_list)
{
	t_list			*list_node;
	t_token_node	*curr_node;

	while (list_node != NULL)
	{
		if (curr_token->type == HEREDOC)
		{
			get_user_input();
		}
		list_node = list_node->next;
	}
}

void	execute_command(t_token *token_list, t_env_list *env_list)
{
	// TODO: 히어독 여러 개 왔을 때 처리하기 << eof cat | << foe cat
	// << eof [cat]-->COMMAND << foe [cat]-->ARGUMENT
	// << eof cat | << foe cat | wc -l
	// heredoc 개수를 세서 미리 임시파일에 저장을 한다.
	// heredoc 일 경우에는 해당 임시파일을 infile fd 로 연결한다.
	// [선택사항] 임시파일을 하나만 만들고 있는데, 이거를 여러 개가 오면 여러 생성할 수 있도록 수정
	// << eof cat | wc -l | << foe cat | wc -l
	check_heredoc(token_list);
	if (get_execute_type(token_list) == SIMPLE_COMMAND)
	{
		if (is_command_builtin_function(token_list) == TRUE)
		{
			process_builtin_function(token_list, env_list, SIMPLE_COMMAND);
		}
		else
		{
			fork_process(token_list, env_list);
		}
	}
	else
	{
		execute_multi_command(token_list, env_list);
	}
}

/*

	1. Readline 에서 스페이스만 들어오는 경우
	2. echo 'hello 
		- 문법 오류
		- 메모리 누수
	3. heredoc
		- ctrl + c (SIGINT): 다시 미니쉘로 돌아와야함 (히어독을 포크로 받으면 됨)
		- ctrl + c (SIGINT) 입력 후 명령어가 실행되면 안됨.
	4. 파이프 닫아주기
		- cat|cat|cat ... lsof | grep minishell 로 확인 가능
	5. ls > a | cat a
		- 클러스터 맥에서 확인해봐야함
	6. echo hi > a | cat a
	7. 파이프 뒤에 히어독
	8. exit code
		- 시그널 처리할 때도 exit code 가 달라짐
		- Child 에서 exit 을 받으면 128 + 시그널 번호 가 반환되어야 함 (cat 으로 입력 받고 있을 때)
	9. PATH 를 unset 하고 ls 를 실행하면 에러 메세지가 떠야함
	
*/