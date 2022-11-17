#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

/*
readline은 여러가지 변수들과 함수들을 사용할 수 있다.

하지만 함수들은 허용된 것들만 사용가능하고 변수는 따로 extern 해주어야 사용이 가능하다.

일단 허용함수들을 살펴보자.

rl_on_new_line : readline실행 중 다음줄로 이동되었다고 설정해주는 함수이다.

rl_replace_line : readline실행 중 들어온 입력을 첫번째 인자로 다시 설정해주는 함수이다.

rl_redisplay : readline실행 중 다시 readline을 실행할 때 사용하는 함수이다.

이들은 signal중 Ctrl + C를 제어할 때 사용된다.
*/
/*
기능: void rl_redisplay (void) 
의 현재 콘텐츠를 반영하도록 화면에 표시되는 내용을 변경 rl_line_buffer합니다.
*/
/*
기능: void rl_replace_line (const char *text, int clear_undo) 
rl_line_buffer의 내용 을 텍스트 로 바꿉니다 . 
포인트와 마크는 가능한 한 보존됩니다. 
clear_undo 가 0이 아닌 경우 현재 행과 연결된 실행 취소 목록이 지워집니다.
*/

/*
rl_replace_line 함수의 역할은 rl_line_buffer에 입력 받은 내용을 text라는 문자열로 대치하는데 있다. 
이 때 clear_undo는 내부적으로 유지 중인 undo_list를 초기화할 지의 여부를 결정 짓는 값이다. 
clear_undo의 값이 0이라면 초기화하지 않고, 0 이외의 값이라면 undo_list를 초기화한다. 
rl_replace_line의 반환 값은 따로 없다.
undo_list는 readline 함수에서 지원하는 Editing 기능 중 Undo 작업에 이용된다.
*/
void    handle_sigint(int sig)
{
	printf("\n");
	rl_on_new_line();
	// 컴파일 시 -lreadline -L ~/.brew/opt/readline/lib -I ~/.brew/opt/readline/include 
	rl_replace_line("", 1);
	rl_redisplay();
}

int main()
{
	char	*prompt;
	char	*input;

	signal(SIGINT, handle_sigint);
	while (42) {
		prompt = getcwd(NULL, 0);
		// Display prompt and read input (n.b. input must be freed after use)...
		input = readline(prompt);

		// Check for EOF.
		if (!input)
			break;

		// Add input to history.
		add_history(input);

		// Do stuff...
		chdir(input);

		// Free input.
		free(input);
	}
}