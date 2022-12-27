#include "builtin.h"
#include "execute.h"

t_bool	is_option_included(char *str)
{
	int	idx;

	idx = 0;
	while (str[idx] != '\0')
	{
		if (str[idx] == '-')
			return (FALSE);
		idx += 1;
	}
	return (TRUE);
}

t_bool	is_valid_variable_name(char *str)
{
	int	idx;

	if (ft_isdigit(str[0]) == TRUE)
		return (FALSE);
	idx = 0;
	while (str[idx] != '\0')
	{
		if (ft_isalnum(str[idx]) == TRUE || str[idx] == '_')
			idx += 1;
		else
			return (FALSE);
	}
	return (TRUE);
}

int	ft_export(char **argv, t_env_list *env_list)
{
	int		idx;
	char	**kv_pair;

	idx = 1;
	kv_pair = NULL;
	(void)env_list;
	while (argv[idx] != NULL)
	{
		kv_pair = ft_split(argv[idx], '=');
		if (is_option_included(kv_pair[0]) == TRUE)
		{
			printf("option\n");
			return (print_error(SYNTAX_ERROR, NULL));
		}
		if (is_valid_variable_name(kv_pair[0]) == FALSE)
		{
			printf("valid\n");
			return (print_error(SYNTAX_ERROR, NULL));
		}
		idx += 1;
	}
	return (EXIT_SUCCESS);
}

/*

	수도 코드
	- argv[1] 부터 시작해서 argv[n] 까지 반복문 돌린다.
	- 옵션이 들어왔는지 확인한다.
	- 환경변수 네이밍 규칙이 유효한지 검사한다.
	- 환경변수의 키만 들어온 경우에는 빈 문자열을 저장한다.
	- 그렇지 않으면 키=값 형태로 저장한다.

	옵션 규칙 확인
	- argv[1 + n] 에서 '-' 로 들어오면 오류 처리

	환경변수 네이밍 규칙
	- 첫 번째 글자에는 문자나 언더바만 올 수 있다.
	- 첫 번째 글자에 숫자는 올 수 없다.
	- 두 번째 글자부터는 문자, 숫자, 언더바 모두 올 수 있다.

	환경 변수 등록
	- 변수명만 들어온 경우에는 빈 문자열로 저장
	- "키=값" 형태로 들어오는 경우에는 정상적으로 저장

*/

/*

	TEST CASES

	export
	> 현재 존재하는 모든 환경 변수들을 보여준다.

	export -f
	> subject 기준으로는 오류 처리

	export a -f
	> subject 기준으로는 오류 처리

	export a
	> "declare -x a" 만 추가 된다.

	export b="hello"
	> env 와 export 를 실행하면 b=hello 가 추가된다.
	
	export mini_a="hello" mini_b="world"
	> 동시에 두 변수가 추가된다.

*/