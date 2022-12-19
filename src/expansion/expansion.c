#include "expansion.h"

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

char	*add_double_quotes(char *env_word)
{
	char	*prev_word;
	char	*buffer;

	buffer = ft_strjoin("\"", env_word);
	prev_word = ft_strdup(buffer);
	free(buffer);
	buffer = ft_strjoin(prev_word, "\"");
	free(prev_word);
	return (buffer);
}

static void	slice_and_save_str(char *str, int start, int idx, char **result)
{
	char	*buffer;
	char	*prev_word;

	buffer = ft_substr(str, start, idx - start);
	prev_word = ft_strdup(*result);
	if (*result != NULL)
		free(*result);
	*result = ft_strjoin(prev_word, buffer);
	free(prev_word);
	if (str[idx] != '\0')
	{
		prev_word = ft_strdup(*result);
		*result = ft_strjoin(prev_word, " ");
		free(prev_word);
	}
}
static char *remove_whitespace(char *str)
{
	int		idx;
	int		start;
	char	*buffer;
	char	*result;
	
	if (str == NULL)
		return (NULL);
	idx = 0;
	result = NULL;
	buffer = ft_strdup(str);
	str = ft_strtrim(buffer, " \n\t\f\v\r");
	free(buffer);
	while (str[idx] != '\0')
	{
		start = idx;
		while (is_whitespace(str[idx]) == FALSE && str[idx] != '\0')
			idx += 1;
		if (start == idx)
		{
			idx += 1;
			continue ;
		}
		slice_and_save_str(str, start, idx, &result);
	}
	return (result);
}

static void	expand_env_variable(char *word, int *idx, t_word_list *word_list, e_quote type)
{
	int		start;
	int		word_length;
	char	*env_word;
	char	*not_spaced_env_word;

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
		if (type == NOT_QUOTED)
		{
			not_spaced_env_word = remove_whitespace(getenv(env_word));
			ft_lstadd_back(&word_list->head_node, ft_lstnew(add_double_quotes(not_spaced_env_word)));
		}
		else
		{
			ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup(getenv(env_word))));
		}
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
			if (curr_token->word[idx] == '\'')
			{
				save_single_quoted_word(curr_token->word, &idx, word_list);
			}

			// 2. 큰 따옴표 안에 있는 문자열
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
						expand_env_variable(curr_token->word, &idx, word_list, QUOTED);
					}
					// $ 만나기 전의 경우
					else
					{
						save_before_env_variable(curr_token->word, &idx, word_list, QUOTED);
					}
				}
				if (curr_token->word[idx] == '\"')
				{
					ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("\"")));
					idx += 1;
				}
			}
			// 3. 따옴표에 둘러 쌓이지 않은 문자열
			else
			{
				while (curr_token->word[idx] != '\0' && curr_token->word[idx] != '\"' && curr_token->word[idx] != '\'')
				{
					// $ 만난 경우
					// 띄어쓰기 제거, 살릴 quote는 살리기
					if (curr_token->word[idx] == '$')
					{
						expand_env_variable(curr_token->word, &idx, word_list, NOT_QUOTED);
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