#include "execute.h"

void	process_redirection(t_list *curr_node, t_redirect *redirect_info)
{
	t_token_node	*curr_token;

	if (redirect_info->file != NONE)
		no_redirect(redirect_info);
	curr_token = curr_node->content;
	if (curr_token->type == REDIR_RIGHT)
		redirect_right(curr_node, redirect_info);
	else if (curr_token->type == REDIR_HEREDOC)
		redirect_heredoc(redirect_info);
	else if (curr_token->type == REDIR_APPEND)
		redirect_append(curr_node, redirect_info);
	else if (curr_token->type == REDIR_LEFT)
		redirect_left(curr_node, redirect_info);
}
