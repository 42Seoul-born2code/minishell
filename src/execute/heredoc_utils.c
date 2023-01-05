#include "minishell.h"
#include "execute.h"

char	*merge_word_list(t_word_list *word_list)
{
	char	*buffer;
	char	*result;
	t_list	*curr_word;
	t_list	*next_word;

	buffer = NULL;
	result = NULL;
	curr_word = word_list->head_node;
	while (curr_word != NULL)
	{
		next_word = curr_word->next;
		buffer = ft_strdup(result);
		if (result != NULL)
			free(result);
		result = ft_strjoin(buffer, (char *)curr_word->content);
		free(buffer);
		free(curr_word->content);
		free(curr_word);
		curr_word = next_word;
	}
	return (result);
}






void	delete_heredoc_file(int heredoc_file_num)
{
	char	*heredoc_file_name;

	while (heredoc_file_num > 0)
	{
		heredoc_file_num -= 1;
		heredoc_file_name = ft_strjoin(HEREDOC_FILE, ft_itoa(heredoc_file_num));
		unlink (heredoc_file_name);
	}
}
