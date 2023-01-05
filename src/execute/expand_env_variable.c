#include "execute.h"

char	*expand_env_variable(char *input)
{
	int				idx;
	int				start;
	int				word_length;
	char			*env_word;
	char			*buffer;
	char			*result;
	t_word_list		*word_list;

	idx = 0;
	word_list = malloc(sizeof(t_word_list));
	word_list->head_node = NULL;
	while (input[idx] != '\0')
	{
		if (input[idx] == '$')
		{
			idx += 1;
			start = idx;
			while (input[idx] != '\0')
			{
				if (is_valid_variable_rule(input[idx]) == FALSE)
					break ;
				if (is_operator(&input[idx]) == TRUE || \
					is_whitespace(input[idx]) == TRUE)
					break ;
				idx += 1;
			}
			if (start == idx)
				ft_lstadd_back(&word_list->head_node, \
						ft_lstnew(ft_strdup("$")));
			else
			{
				word_length = idx - start;
				env_word = malloc(sizeof(char) * (word_length + 1));
				ft_memcpy(env_word, &input[start], word_length);
				ft_lstadd_back(&word_list->head_node, \
						ft_lstnew(ft_strdup(getenv(env_word))));
				free(env_word);
			}
		}
		else
		{
			start = idx;
			while (input[idx] != '\0' && input[idx] != '$')
				idx += 1;
			word_length = idx - start;
			buffer = malloc(sizeof(char) * (word_length + 1));
			ft_memcpy(buffer, &input[start], word_length);
			ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup(buffer)));
			free(buffer);
		}
	}
	result = merge_word_list(word_list);
	free(word_list);
	return (result);
}