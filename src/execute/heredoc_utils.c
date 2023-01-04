#include "execute.h"

void	check_heredoc(t_token *token_list)
{
	t_list			*curr_node;
	t_token_node	*limiter_node;
	t_token_node	*curr_token;
	int				idx;

	idx = 0;
	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		if (curr_token->type == REDIR_HEREDOC)
		{
			limiter_node = curr_node->next->content;
			get_user_input(limiter_node->word, idx);
			idx += 1;
		}
		curr_node = curr_node->next;
	}
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
