/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@studnet.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 16:35:57 by joonhan           #+#    #+#             */
/*   Updated: 2022/08/25 17:00:24 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*split_newline(t_node *p_node, int i)
{
	char	*next;
	char	*newline;
	ssize_t	backup_len;

	next = NULL;
	newline = (char *)malloc(sizeof(char) * (i + 2));
	if (newline == NULL)
		return (NULL);
	newline = ft_memcpy(newline, p_node->backup, i + 1);
	backup_len = ft_strlen(p_node->backup);
	if (backup_len - i > 1)
	{
		next = (char *)malloc(sizeof(char) * (backup_len - i));
		if (next == NULL)
			return (NULL);
		next = ft_memcpy(next, &p_node->backup[i + 1], backup_len - i - 1);
	}
	if (p_node->backup != NULL)
		free(p_node->backup);
	if (next != NULL)
		p_node->backup = next;
	else
		p_node->backup = NULL;
	return (newline);
}

static void	save_in_backup(t_node **p_head, t_node *p_node, ssize_t len)
{
	char	*buf;
	char	*prev_backup;

	buf = (char *)malloc(sizeof(char) * (len + 1));
	if (buf == NULL)
		free_fd(p_head, p_node->fd);
	buf = ft_memcpy(buf, p_node->buf, len);
	if (p_node->backup == NULL)
		p_node->backup = buf;
	else
	{
		len = ft_strlen(p_node->backup);
		prev_backup = (char *)malloc(sizeof(char) * (len + 1));
		if (prev_backup == NULL)
		{
			free(buf);
			free_fd(p_head, p_node->fd);
			return ;
		}
		prev_backup = ft_memcpy(prev_backup, p_node->backup, len);
		free(p_node->backup);
		p_node->backup = ft_strjoin(prev_backup, buf);
		free(prev_backup);
		free(buf);
	}
}

static int	check_newline_in_backup(t_node **p_head, t_node *p_node, \
									ssize_t len)
{
	int	i;
	int	found;

	if (len > 0)
		save_in_backup(p_head, p_node, len);
	i = 0;
	found = FALSE;
	while (p_node->backup != NULL && p_node->backup[i] != '\0')
	{
		if (p_node->backup[i] == '\n')
		{
			found = TRUE;
			break ;
		}
		i += 1;
	}
	if (found == FALSE)
		return (FALSE);
	return (i);
}

static char	*clear_backup(t_node **p_head, t_node *p_node, int fd)
{
	char	*newline;
	ssize_t	backup_len;

	if (p_node->backup == NULL)
		return (free_fd(p_head, fd));
	backup_len = ft_strlen(p_node->backup);
	newline = (char *)malloc(sizeof(char) * (backup_len + 1));
	if (newline == NULL)
		return (free_fd(p_head, fd));
	newline = ft_memcpy(newline, p_node->backup, backup_len);
	free_fd(p_head, fd);
	return (newline);
}

char	*get_next_line(int fd)
{
	ssize_t			len;
	t_node			*p_node;
	static t_node	*p_head;
	int				found_newline;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	p_node = find_fd(&p_head, fd);
	if (p_head == NULL || p_node == NULL)
		return (free_fd(&p_head, fd));
	len = 0;
	while (len >= 0)
	{
		len = read(fd, p_node->buf, BUFFER_SIZE);
		if (len < 0)
			return (free_fd(&p_head, fd));
		found_newline = check_newline_in_backup(&p_head, p_node, len);
		if (found_newline != FALSE)
			return (split_newline(p_node, found_newline));
		if (len != BUFFER_SIZE)
			return (clear_backup(&p_head, p_node, fd));
	}
	return (NULL);
}
