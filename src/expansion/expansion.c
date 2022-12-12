#include "expansion.h"

/*

	echo "hello $NAME"
	echo $NAME
	''

	export a=ho
	ec$a $NAME

	1. word 에 큰 따옴표가 있는 경우
		- 환경 변수 이름 파악 (whitespace 나 operator 전까지)
		- 환경 변수에서 값을 가져오기. 없으면 빈 문자열 처리
	2. word 에 큰 따옴표가 없는 경우
		- 작은 따옴표 안에 있는 환경 변수는 치환하지 않는다.
		- 작은 따옴표를 만났는지 확인하는 플래그 변수 사용하기

	echo 'hello "$NAME"'
		hello "$NAME"

	echo "'hello "$NAME"'"
		'hello joonhyeok.han'

	echo "'hello '$NAME''"
		'hello 'joonhyeok.han''

	echo ""$NAME" '$NAME'"
		joonhyeok.han 'joonhyeok.han'

	echo hello,"$NAME",welcome
*/


// 'hello "$word"'

// 
void	expansion(t_token *token_list)
{
	int				i;
	int				start;
	int				word_length;
	char			*prev_word;
	char			*env_word;
	t_list			*curr_node;
	t_token_node	*curr_token;
	t_bool			is_quote_open;
	// char			start_quote_type;

	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		i = 0;
		is_quote_open = FALSE;
		// start_quote_type = '\0';
		curr_token = curr_node->content;
		// word 를 순회하면서 따옴표 안에 있는 환경 변수를 치환
		while (curr_token->word[i] != '\0')
		{
			// 1. 작은 따옴표 안에 있는 문자열
			// - 닫는 따옴표 다시 만나기 전까지 인덱스 증가
			// - 작은 따옴표 출력 X
			// - CASE1: echo 'hello "$NAME"'
			// - CASE2: echo hello'world"$NAME"'
			if (is_quote_open == FALSE && curr_token->word[i] == '\'')
			{
				i += 1;
				while (curr_token->word[i] != '\0' && curr_token->word[i] != '\'')
					i += 1;
			}

			// 2. 큰 따옴표 안에 있는 문자열
			// - 작은 따옴표에 둘러 쌓여 있어도 무조건 치환
			// - 작은 따옴표도 출력이 돼야함
			// - CASE1: echo "hello"
			// - CASE2: echo "hello $NAME"
			//               0123456789
			// echo "hello $NAME superstar >> $INTRA" (NAME = joon, INTRA = joonhan)
			// echo "hello joon joonhan"
			// echo "hello $NAME "
			// - CASE2: echo "hello'$NAME'world"
			else if (is_quote_open == FALSE && curr_token->word[i] == '\"')
			{
				// start_quote_type = '\"';
				start = i;
				is_quote_open = TRUE;
				i += 1;
				// 치환이 이루어지는 과정
				while (curr_token->word[i] != '\0' && curr_token->word[i] != '\"')
				{
					if (curr_token->word[i] == '$')
					{
						// 이전까지 문자열 저장
						word_length = i - start;
						prev_word = malloc(sizeof(char) * (word_length + 1));
						ft_memcpy(prev_word, &curr_token->word[start], word_length);
						i += 1;
						// echo "hello $NAME"
						// 환경변수 이름 저장
						start = i;
						// TODO: $NAME$NAME 떄문에 $조건 추가해줘야함
						while (curr_token->word[i] != '\"')
						{
							if (is_operator(&curr_token->word[i]) == TRUE || is_whitespace(curr_token->word[i]) == TRUE)
							{
								break ;
							}
							i += 1;
						}
						word_length = i - start;
						env_word = malloc(sizeof(char) * (word_length + 1));
						ft_memcpy(env_word, &curr_token->word[start], word_length);
						printf("getenv: %s\n", getenv(env_word));
					}
					i += 1;
				}
			}
			else
			{
				i += 1;
			}
			// 3. 따옴표에 둘러 쌓이지 않은 문자열
			// - 중간에 큰 따옴표를 만나면 2번 적용
			// - CASE1: echo hello,"$NAME"
		}
		curr_node = curr_node->next;
	}
}