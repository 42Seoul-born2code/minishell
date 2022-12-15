#include "expansion.h"

/*

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

// echo "'$PATH"' -- trash value in $PATH [ 해결 ]
// echo ls" " -- 공백이 들어오면 개복치마냥 터져버림

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

	ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("\'")));
	*idx += 1;
	start = *idx;
	while (word[*idx] != '\0' && word[*idx] != '\'')
		*idx += 1;
	if (start == *idx)
		return ;
	else
	{
		word_length = *idx - start;
		buffer = malloc(sizeof(char) * (word_length + 1));
		ft_memcpy(buffer, &word[start], word_length);
		ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup(buffer)));
		ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("\'")));
		free(buffer);
		*idx += 1;
	}
}

// 환경변수를 확장하기 위해 연결 리스트로 저장했던 문자열들을 하나로 합치는 함수
// replaced_word 에 char ** 를 사용한 이유는 char * 로 값을 바꾸면 expansion 함수에 변경 내용이 반영되지 않음
void	merge_replaced_word(t_word_list *word_list, char **replaced_word)
{	
	char	*previous_word;
	t_list	*curr_word;
	t_list	*next_word;

	curr_word = word_list->head_node;
	while (curr_word != NULL)
	{
		next_word = curr_word->next;
		previous_word = ft_strdup(*replaced_word);
		free(*replaced_word);
		*replaced_word = ft_strjoin(previous_word, (char *)curr_word->content);
		free(curr_word->content);
		free(curr_word);
		curr_word = next_word;
	}
}

// 환경 변수를 확장해서 기존의 토큰을 교체하는 함수
void	expansion(t_token *token_list)
{
	int				idx;
	char			*replaced_word;
	t_list			*curr_node;
	t_token_node	*curr_token;
	t_word_list		*word_list;

	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		replaced_word = NULL;
		word_list = malloc(sizeof(t_word_list));
		word_list->head_node = NULL;
		idx = 0;
		curr_token = curr_node->content;
		// 현재 토큰에 담긴 문자열의 처음부터 끝까지 순회
		while (curr_token->word[idx] != '\0')
		{
			// 1. 작은 따옴표 안에 있는 문자열
			// - 닫는 따옴표 다시 만나기 전까지 인덱스 증가
			// - 작은 따옴표 출력 X
			// - CASE1: echo 'hello "$NAME"'
			// - CASE2: echo hello'world"$NAME"'
			if (curr_token->word[idx] == '\'')
			{
				save_single_quoted_word(curr_token->word, &idx, word_list);
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
			else if (curr_token->word[idx] == '\"')
			{
				ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("\"")));
				idx += 1;
				// 치환이 이루어지는 과정
				while (curr_token->word[idx] != '\0' && curr_token->word[idx] != '\"')
				{
					// $ 만난 경우
					if (curr_token->word[idx] == '$')
					{
						expand_env_variable(curr_token->word, &idx, word_list);
					}
					// $ 만나기 전의 경우
					else
					{
						save_before_env_variable(curr_token->word, &idx, word_list, QUOTED);
					}
					if (curr_token->word[idx] == '\"')
					{
						idx += 1;
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
				while (curr_token->word[idx] != '\0' && curr_token->word[idx] != '\"' && curr_token->word[idx] != '\'')
				{
					// $ 만난 경우
					if (curr_token->word[idx] == '$')
					{
						expand_env_variable(curr_token->word, &idx, word_list);
					}
					// $ 만나기 전의 경우
					else
					{
						save_before_env_variable(curr_token->word, &idx, word_list, NOT_QUOTED);
					}
				}
			}
		}
		// 연결 리스트 노드들 하나로 합치기
		merge_replaced_word(word_list, &replaced_word);
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