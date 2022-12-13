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
	char			*env_word;
	char			*buffer;
	char			*replaced_word;
	t_list			*curr_node;
	t_token_node	*curr_token;
	t_bool			is_quote_open;

	curr_node = token_list->head_node;
	buffer = NULL;
	while (curr_node != NULL)
	{
		i = 0;
		is_quote_open = FALSE;
		curr_token = curr_node->content;
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
			// - CASE2: echo "hello $NAME$"
			//               0123456789
			// - CASE3: echo "$NAME$NAME"
			//               0123456789
			// echo "hello $NAME superstar >> $INTRA" (NAME = joon, INTRA = joonhan)
			// echo "hello joon joonhan"
			// echo "hello $NAME "
			// - CASE2: echo "hello'$NAME'world"
			else if (is_quote_open == FALSE && curr_token->word[i] == '\"')
			{
				replaced_word = "\"";
				start = i;
				i += 1;
				is_quote_open = TRUE;
				// 치환이 이루어지는 과정
				while (curr_token->word[i] != '\0' && curr_token->word[i] != '\"')
				{
					// $ 만난 경우
					if (curr_token->word[i] == '$')
					{
						i += 1;
						start = i;
						// TODO: $NAME$NAME 떄문에 $조건 추가해줘야함
						while (curr_token->word[i] != '\"' && curr_token->word[i] != '$')
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
						replaced_word = ft_strjoin(replaced_word, getenv(env_word));
						free(env_word);
						printf("if>> replaced_word: %s\n", replaced_word);
						
					}
					// $ 만나기 전의 경우
					else
					{
						start = i;
						while (curr_token->word[i] != '\0' && curr_token->word[i] != '\"' && curr_token->word[i] != '$')
						{
							i += 1;
						}
						word_length = i - start;
						buffer = malloc(sizeof(char) * (word_length + 1));
						ft_memcpy(buffer, &curr_token->word[start], word_length);
						replaced_word = ft_strjoin(replaced_word, buffer);
						free(buffer);
						printf("else>> replaced_word: %s\n", replaced_word);
					}
				}
				replaced_word = ft_strjoin(replaced_word, "\"");
				printf("END>> replaced_word: %s\n", replaced_word);
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