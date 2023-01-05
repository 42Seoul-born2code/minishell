#include "minishell.h"
#include "expansion.h"

static t_quote	get_content_quote_type(char *str)
{
	int	idx;

	if (str == NULL)
		return (NOT_QUOTED);
	idx = 0;
	while (str[idx] != '\0')
	{
		if (str[idx] == '\'')
			return (SINGLE_QUOTED);
		if (str[idx] == '\"')
			return (DOUBLE_QUOTED);
		idx += 1;
	}
	return (NOT_QUOTED);
}

static char	*add_quotes(char *env_word)
{
	t_quote	quote_type;
	char	*prev_word;
	char	*buffer;

	quote_type = get_content_quote_type(env_word);
	if (quote_type == DOUBLE_QUOTED)
		buffer = ft_strjoin("\'", env_word);
	else
		buffer = ft_strjoin("\"", env_word);
	prev_word = ft_strdup(buffer);
	free(buffer);
	if (quote_type == DOUBLE_QUOTED)
		buffer = ft_strjoin(prev_word, "\'");
	else
		buffer = ft_strjoin(prev_word, "\"");
	free(prev_word);
	return (buffer);
}

static void	lst_add_back_expanded_word(char *env_word, t_quote quote_type, \
						t_word_info word_info, t_word_list *word_list)
{
	char		*not_spaced_env_word;
	t_meta		type;
	t_env_list	*env_list;

	type = word_info.type;
	env_list = word_info.env_list;
	if (type == LIMITER)
		ft_lstadd_back(&word_list->head_node, \
			ft_lstnew(ft_strjoin("$", env_word)));
	else if (quote_type == NOT_QUOTED)
	{
		not_spaced_env_word = \
			remove_whitespace(ft_strdup(get_env_value(env_list, env_word)));
		ft_lstadd_back(&word_list->head_node, \
			ft_lstnew(add_quotes(not_spaced_env_word)));
		free(not_spaced_env_word);
	}
	else if (quote_type == QUOTED)
		ft_lstadd_back(&word_list->head_node, \
			ft_lstnew(ft_strdup(get_env_value(env_list, env_word))));
	free(env_word);
}

void	move_word_idx(char *word, int *idx)
{
	while (word[*idx] != '\0' && word[*idx] != '\"' && \
			word[*idx] != '\'' && word[*idx] != '$')
	{
		if (is_valid_variable_rule(word[*idx]) == FALSE)
			break ;
		if (is_operator(&word[*idx]) == TRUE || \
			is_whitespace(word[*idx]) == TRUE)
			break ;
		*idx += 1;
	}
}

void	save_expand_env_variable(t_word_info word_info, int *idx, \
					t_word_list *word_list, t_quote quote_type)
{
	int		start;
	char	*word;
	char	*env_word;

	word = word_info.word;
	*idx += 1;
	start = *idx;
	move_word_idx(word, idx);
	if (start == *idx && word[start - 1] == '$' && word[*idx] != '?')
		ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup("$")));
	else if (word[*idx] == '?')
	{
		ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_itoa(g_exit_code)));
		*idx += 1;
	}
	else if ((start == *idx && word[*idx] == '\0') == FALSE)
	{
		env_word = copy_str_length(start, word, *idx - start);
		lst_add_back_expanded_word(env_word, quote_type, word_info, word_list);
	}
}
