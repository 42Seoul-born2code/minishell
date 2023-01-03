#include "builtin.h"

t_bool	is_option_included(char *str)
{
	int	idx;

	idx = 0;
	while (str[idx] != '\0')
	{
		if (str[idx] == '-')
			return (TRUE);
		idx += 1;
	}
	return (FALSE);
}

int	print_export_env_list(t_env_list *env_list)
{
	t_list		*list_node;
	t_env_node	*curr_node;

	list_node = env_list->head_node;
	while (list_node != NULL)
	{
		curr_node = list_node->content;
		if (curr_node->value == NULL)
			printf("declare -x %s\n", curr_node->key);
		else
			printf("declare -x %s=%s\n", curr_node->key, curr_node->value);
		list_node = list_node->next;
	}
	return (EXIT_SUCCESS);
}

void	ft_lstadd_env_node(t_env_list *env_list, char *key, char *value)
{
	t_env_node	*new_node;

	new_node = malloc(sizeof(t_env_node));
	new_node->key = key;
	new_node->value = value;
	ft_lstadd_back(&env_list->head_node, ft_lstnew(new_node));
}

void	save_export(t_env_list *env_list, char **kv_pair)
{
	char		*key;
	char		*value;
	t_list		*list_node;
	t_env_node	*curr_node;

	list_node = env_list->head_node;
	key = ft_strdup(kv_pair[KEY]);
	if (kv_pair[VALUE] == NULL)
		value = ft_strdup("");
	else
		value = ft_strdup(kv_pair[VALUE]);
	while (list_node != NULL)
	{
		curr_node = list_node->content;
		if (ft_strcmp(curr_node->key, kv_pair[KEY]) == 0)
		{
			free(curr_node->key);
			free(curr_node->value);
			curr_node->key = key;
			curr_node->value = value;
			return ;
		}
		list_node = list_node->next;
	}
	ft_lstadd_env_node(env_list, key, value);
}

int	ft_export(char **argv, t_env_list *env_list)
{
	int		idx;
	char	**kv_pair;

	if (argv[1] == NULL)
		return (print_export_env_list(env_list));
	idx = 1;
	kv_pair = NULL;
	while (argv[idx] != NULL)
	{
		kv_pair = ft_split(argv[idx], '=');
		if (is_option_included(kv_pair[KEY]) == TRUE)
			return (print_error(SYNTAX_ERROR, NULL));
		if (is_valid_variable_name(kv_pair[KEY]) == FALSE)
			return (print_error(SYNTAX_ERROR, NULL));
		if (get_env_value(env_list, kv_pair[KEY]) == FALSE)
		{
			save_export(env_list, kv_pair);
		}
		free_all(kv_pair);
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