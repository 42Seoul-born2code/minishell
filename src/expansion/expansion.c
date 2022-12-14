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
static	t_bool is_valid_variable_rule(char c)
{
	if (ft_isalnum(c) == TRUE || c == '_')
		return (TRUE);
	return (FALSE);
}

static void	save_before_env_variable(char *word, int *idx, t_word_list *word_list, e_quote type)
{
	int		start;
	int		word_length;
	char	*buffer;
	t_bool	is_stopable;

	start = *idx;
	is_stopable = FALSE;
	// echo hello'world"$NAME"'
	// echo hello"$NAME"
	while (word[*idx] != '\0')
	{
		if (type == NOT_QUOTED)
			is_stopable = word[*idx] == '\'' || word[*idx] == '\"' || word[*idx] == '$';
		else
			is_stopable = word[*idx] == '\"' || word[*idx] == '$';
		if (is_stopable == TRUE)
			break ;
		*idx += 1;
	}
	word_length = *idx - start;
	buffer = malloc(sizeof(char) * (word_length + 1));
	ft_memcpy(buffer, &word[start], word_length);
	ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup(buffer)));
	free(buffer);
}

static void	expand_env_variable(char *word, int *idx, t_word_list *word_list)
{
	int		start;
	int		word_length;
	char	*env_word;

	*idx += 1;
	start = *idx;
	while (word[*idx] != '\0' && word[*idx] != '\"' && word[*idx] != '\'' && word[*idx] != '$')
	{
		if (is_valid_variable_rule(word[*idx]) == FALSE)
			break ;
		if (is_operator(&word[*idx]) == TRUE || is_whitespace(word[*idx]) == TRUE)
			break ;
		*idx += 1;
	}
	if (start == *idx)
		ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("$")));
	else
	{
		word_length = *idx - start;
		env_word = malloc(sizeof(char) * (word_length + 1));
		ft_memcpy(env_word, &word[start], word_length);
		ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup(getenv(env_word))));
		free(env_word);
	}
}

static void	save_single_quoted_word(char *word, int *idx, t_word_list *word_list)
{
	int		start;
	int		word_length;
	char	*buffer;

	start = *idx;
	*idx += 1;
	while (word[*idx] != '\0' && word[*idx] != '\'')
	{
		*idx += 1;
	}
	word_length = *idx - start;
	buffer = malloc(sizeof(char) * (word_length + 1));
	ft_memcpy(buffer, &word[start], word_length);
	ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup(buffer)));
	ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("\'")));
	free(buffer);
	*idx += 1;
}

// 환경 변수를 확장해서 기존의 토큰을 교체하는 함수
void	expansion(t_token *token_list)
{
	int				i;
	char			*temp_word;
	char			*replaced_word;
	t_list			*curr_node;
	t_token_node	*curr_token;
	t_word_list		*word_list;
	t_list			*curr_word;
	t_list			*next_word;

	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		replaced_word = NULL;
		word_list = malloc(sizeof(t_word_list));
		word_list->head_node = NULL;
		i = 0;
		curr_token = curr_node->content;
		// 현재 토큰에 담긴 문자열의 처음부터 끝까지 순회
		while (curr_token->word[i] != '\0')
		{
			// 1. 작은 따옴표 안에 있는 문자열
			// - 닫는 따옴표 다시 만나기 전까지 인덱스 증가
			// - 작은 따옴표 출력 X
			// - CASE1: echo 'hello "$NAME"'
			// - CASE2: echo hello'world"$NAME"'
			if (curr_token->word[i] == '\'')
			{
				save_single_quoted_word(curr_token->word, &i, word_list);
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
			// - CASE4: echo "hello'$NAME'world"
			// - CASE5: echo "hello, $NAME.hi"hi
			else if (curr_token->word[i] == '\"')
			{
				ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("\"")));
				i += 1;
				// 치환이 이루어지는 과정
				while (curr_token->word[i] != '\0' && curr_token->word[i] != '\"')
				{
					// $ 만난 경우
					if (curr_token->word[i] == '$')
					{
						expand_env_variable(curr_token->word, &i, word_list);
					}
					// $ 만나기 전의 경우
					else
					{
						save_before_env_variable(curr_token->word, &i, word_list, QUOTED);
					}
					if (curr_token->word[i] == '\"')
					{
						i += 1;
						break ;
					}
				}
				ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("\"")));
			}
			// 3. 따옴표에 둘러 쌓이지 않은 문자열
			// - 중간에 큰 따옴표를 만나면 2번 적용
			// export a=ho
			// ec$a hello,$NAME
			// - CASE1: echo hello,$NAME
			// - CASE2: echo hello'world"$NAME"'
			else
			{
				while (curr_token->word[i] != '\0' && curr_token->word[i] != '\"' && curr_token->word[i] != '\'')
				{
					// $ 만난 경우
					if (curr_token->word[i] == '$')
					{
						expand_env_variable(curr_token->word, &i, word_list);
					}
					// $ 만나기 전의 경우
					else
					{
						save_before_env_variable(curr_token->word, &i, word_list, NOT_QUOTED);
					}
				}
			}
		}
		// 연결 리스트 노드들 하나로 합치기
		curr_word = word_list->head_node;
		while (curr_word != NULL)
		{
			next_word = curr_word->next;
			temp_word = ft_strdup(replaced_word);
			free(replaced_word);
			replaced_word = ft_strjoin(temp_word, (char *)curr_word->content);
			free(curr_word->content);
			free(curr_word);
			curr_word = next_word;
		}
		// 기존에 있던 word 를 교체
		if (replaced_word != NULL)
		{
			free(curr_token->word);
			curr_token->word = replaced_word;
		}
		free(word_list);
		printf("curr_token->word: %s\n", curr_token->word);
		curr_node = curr_node->next;
	}
}