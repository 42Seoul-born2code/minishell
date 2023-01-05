#include "execute.h"

void	process_redirection(t_list *curr_node, t_redirect *redirect_info)
{
	t_token_node	*curr_token;

	curr_token = curr_node->content;
	if (curr_token->type == REDIR_RIGHT || curr_token->type == REDIR_APPEND)
	{
		if (redirect_info->outfile != NONE)
			close(redirect_info->outfile);
		if (curr_token->type == REDIR_RIGHT)
			redirect_right(curr_node, redirect_info);
		else if (curr_token->type == REDIR_APPEND)
			redirect_append(curr_node, redirect_info);
	}
	else
	{
		if (redirect_info->infile != NONE)
			close(redirect_info->infile);
		if (curr_token->type == REDIR_LEFT)
			redirect_left(curr_node, redirect_info);
		else if (curr_token->type == REDIR_HEREDOC)
			redirect_heredoc(redirect_info);
	}
}
