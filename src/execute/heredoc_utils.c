/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:17:52 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/06 16:18:29 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	get_heredoc_file_fd(int heredoc_idx, int mode)
{
	int		fd;
	char	*heredoc_file_name;
	char	*heredoc_file_idx;

	fd = NONE;
	heredoc_file_idx = ft_itoa(heredoc_idx);
	heredoc_file_name = ft_strjoin(HEREDOC_FILE, heredoc_file_idx);
	if (mode == READ_MODE)
		fd = open(heredoc_file_name, O_RDONLY, 0644);
	else if (mode == WRITE_MODE)
		fd = open(heredoc_file_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	free(heredoc_file_idx);
	free(heredoc_file_name);
	return (fd);
}

void	delete_heredoc_file(int heredoc_file_num)
{
	char	*heredoc_file_name;
	char	*heredoc_file_idx;

	while (heredoc_file_num > 0)
	{
		heredoc_file_num -= 1;
		heredoc_file_idx = ft_itoa(heredoc_file_num);
		heredoc_file_name = ft_strjoin(HEREDOC_FILE, heredoc_file_idx);
		free(heredoc_file_name);
		free(heredoc_file_idx);
		unlink (heredoc_file_name);
	}
}
