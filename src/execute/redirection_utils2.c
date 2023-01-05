#include "execute.h"

void	no_redirect(t_redirect *redirect_info)
{
	close(redirect_info->file);
	if (redirect_info->type == OUTFILE)
		close(STDOUT_FILENO);
	else if (redirect_info->type == INFILE)
		close(STDIN_FILENO);
}

// << eof : 
// > outfile
void	redirect_right(t_list *curr_node, t_redirect *redirect_info)
{
	redirect_info->file = open_file(curr_node->next, WRITE_MODE);
	redirect_info->type = OUTFILE;
	dup2(redirect_info->file, STDOUT_FILENO);
}

void	redirect_heredoc(t_redirect *redirect_info)
{
	redirect_info->file = get_heredoc_file_fd(redirect_info->heredoc_file_num, \
												READ_MODE);
	redirect_info->type = HEREDOC;
	dup2(redirect_info->file, STDIN_FILENO);
	redirect_info->heredoc_file_num += 1;
}

void	redirect_append(t_list *curr_node, t_redirect *redirect_info)
{
	redirect_info->file = open_file(curr_node->next, APPEND_MODE);
	redirect_info->type = OUTFILE;
	dup2(redirect_info->file, STDOUT_FILENO);
}

void	redirect_left(t_list *curr_node, t_redirect *redirect_info)
{
	redirect_info->file = open_file(curr_node->next, READ_MODE);
	redirect_info->type = INFILE;
	dup2(redirect_info->file, STDIN_FILENO);
}
