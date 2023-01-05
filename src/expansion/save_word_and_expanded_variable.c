#include "expansion.h"

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

/*
	1. $ 만난 경우: 환경변수 확장해서 저장
	2. $ 만나기 전의 경우: 환경변수 만나기 전까지 문자열 저장
*/
static void	add_back_to_word_list(t_token_node *curr_token, int *idx, t_word_list *word_list, t_env_list *env_list, t_quote quote_type)
{
	if (curr_token->word[*idx] == '$')
	{
		save_expand_env_variable(curr_token, idx, word_list, env_list, quote_type);
	}
	else
	{
		save_before_env_variable(curr_token->word, idx, word_list, quote_type);
	}
}

static void	save_double_quoted_word(t_token_node *curr_token, int *idx, t_word_list *word_list, t_env_list *env_list)
{
	ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("\"")));
	*idx += 1;
	while (curr_token->word[*idx] != '\0' && curr_token->word[*idx] != '\"')
	{
		add_back_to_word_list(curr_token, idx, word_list, env_list, QUOTED);
	}
	if (curr_token->word[*idx] == '\"')
	{
		ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("\"")));
		*idx += 1;
	}
}

static void	save_unquoted_word(t_token_node *curr_token, int *idx, t_word_list *word_list, t_env_list *env_list)
{
	while (curr_token->word[*idx] != '\0' && curr_token->word[*idx] != '\"' && curr_token->word[*idx] != '\'')
		add_back_to_word_list(curr_token, idx, word_list, env_list, NOT_QUOTED);
}

t_word_list	*save_word_and_expanded_variable(t_token_node *curr_token, t_env_list *env_list)
{
	int				idx;
	t_word_list		*word_list;

	idx = 0;
	word_list = malloc(sizeof(t_word_list));
	word_list->head_node = NULL;
	while (curr_token->word[idx] != '\0')
	{
		if (curr_token->word[idx] == '\'')
			save_single_quoted_word(curr_token->word, &idx, word_list);
		else if (curr_token->word[idx] == '\"')
			save_double_quoted_word(curr_token, &idx, word_list, env_list);
		else
			save_unquoted_word(curr_token, &idx, word_list, env_list);
	}
	return (word_list);
}
