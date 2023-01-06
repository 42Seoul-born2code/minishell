/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:18:04 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/06 16:18:14 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execute.h"

void	redirect_right(t_list *curr_node, t_redirect *redirect_info)
{
	redirect_info->outfile = open_file(curr_node->next, WRITE_MODE);
	if (redirect_info->outfile == ERROR)
		g_exit_code = 1;
	redirect_info->type = OUTFILE;
	dup2(redirect_info->outfile, STDOUT_FILENO);
}

void	redirect_append(t_list *curr_node, t_redirect *redirect_info)
{
	redirect_info->outfile = open_file(curr_node->next, APPEND_MODE);
	if (redirect_info->outfile == ERROR)
		g_exit_code = 1;
	redirect_info->type = OUTFILE;
	dup2(redirect_info->outfile, STDOUT_FILENO);
}

void	redirect_left(t_list *curr_node, t_redirect *redirect_info)
{
	redirect_info->infile = open_file(curr_node->next, READ_MODE);
	if (redirect_info->infile == ERROR)
		g_exit_code = 1;
	redirect_info->type = INFILE;
	dup2(redirect_info->infile, STDIN_FILENO);
}

void	redirect_heredoc(t_redirect *redirect_info)
{
	redirect_info->infile = get_heredoc_file_fd(\
		redirect_info->heredoc_file_num, READ_MODE);
	if (redirect_info->infile == ERROR)
		g_exit_code = 1;
	redirect_info->type = HEREDOC;
	dup2(redirect_info->infile, STDIN_FILENO);
	redirect_info->heredoc_file_num += 1;
}
