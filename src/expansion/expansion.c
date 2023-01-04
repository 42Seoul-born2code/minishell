#include "minishell.h"
#include "expansion.h"
#include "utils.h"

t_bool	is_valid_variable_rule(char c)
{
	if (ft_isalnum(c) == TRUE || c == '_')
		return (TRUE);
	return (FALSE);
}

static void	save_before_env_variable(\
char *word, int *idx, t_word_list *word_list, t_quote type)
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
	free(buffer);
	if (str[idx] != '\0')
	{
		prev_word = ft_strdup(*result);
		free(*result);
		*result = ft_strjoin(prev_word, " ");
		free(prev_word);
	}
}

static char	*remove_whitespace(char *str)
{
	int		idx;
	int		start;
	char	*buffer;
	char	*result;

	if (str == NULL)
		return (NULL);
	idx = 0;
	result = NULL;
	buffer = ft_strtrim(str, " \n\t\f\v\r");
	while (str[idx] != '\0')
	{
		start = idx;
		while (is_whitespace(buffer[idx]) == FALSE && buffer[idx] != '\0')
			idx += 1;
		if (start == idx)
		{
			idx += 1;
			continue ;
		}
		slice_and_save_str(buffer, start, idx, &result);
	}
	free(buffer);
	free(str);
	return (result);
}

static void	expand_env_variable(t_token_node *token, int *idx, \
					t_word_list *word_list, t_quote quote_type, t_env_list *env_list)
{
	int		start;
	int		word_length;
	char	*env_word;
	char	*not_spaced_env_word;

	*idx += 1;
	start = *idx;
	while (token->word[*idx] != '\0' && token->word[*idx] != '\"' && token->word[*idx] != '\'' && token->word[*idx] != '$')
	{
		if (is_valid_variable_rule(token->word[*idx]) == FALSE)
			break ;
		if (is_operator(&token->word[*idx]) == TRUE || is_whitespace(token->word[*idx]) == TRUE)
			break ;
		*idx += 1;
	}
	if (start == *idx && token->word[start - 1] == '$')
	{
		ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("$")));
	}
	else if (start == *idx && token->word[*idx] == '\0')
		return ;
	else if (token->word[*idx] == '?')
	{
		ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_itoa(g_exit_code)));
		*idx += 1;
	}
	else
	{
		word_length = *idx - start;
		env_word = malloc(sizeof(char) * (word_length + 1));
		ft_memcpy(env_word, &token->word[start], word_length);
		if (token->type == LIMITER)
			ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strjoin("$", env_word)));
		else if (quote_type == NOT_QUOTED)
		{
			not_spaced_env_word = remove_whitespace(ft_strdup(get_env_value(env_list, env_word)));
			ft_lstadd_back(&word_list->head_node, ft_lstnew(add_double_quotes(not_spaced_env_word)));
			free(not_spaced_env_word);
		}
		else if (quote_type == QUOTED)
			ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup(get_env_value(env_list, env_word))));
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
	if (start == *idx && word[*idx] == '\'')
	{
		ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("\'")));
		*idx += 1;
	}
	else if (start == *idx && word[*idx] == '\0')
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
		free(previous_word);
		free(curr_word->content);
		free(curr_word);
		curr_word = next_word;
	}
}

// 환경 변수를 확장해서 기존의 토큰을 교체하는 함수
void	expansion(t_token *token_list, t_env_list *env_list)
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
				ft_lstadd_back(
					&word_list->head_node, ft_lstnew(ft_strdup("\"")));
				idx += 1;
				// 치환이 이루어지는 과정
				while (curr_token->word[idx] != '\0' && curr_token->word[idx] != '\"')
				{
					// $ 만난 경우
					if (curr_token->word[idx] == '$')
					{
						expand_env_variable(curr_token, &idx, word_list, QUOTED, env_list);
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
						expand_env_variable(curr_token, &idx, word_list, NOT_QUOTED, env_list);
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
			// printf("replaced_word: %s\n", replaced_word);
			curr_token->word = replaced_word;
		}
		free(word_list);
		// printf("curr_token->word: %s\n", curr_token->word);
		curr_node = curr_node->next;
	}
}

// echo $USER$NOT$$"h"w''$
// echo '"$USER"'$USER