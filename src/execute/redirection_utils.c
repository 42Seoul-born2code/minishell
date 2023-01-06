/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:18:03 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/06 16:18:04 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	save_origin_fd(int origin_fd[2])
{
	origin_fd[0] = dup(STDIN_FILENO);
	origin_fd[1] = dup(STDOUT_FILENO);
}

void	rollback_origin_fd(int origin_fd[2])
{
	dup2(origin_fd[0], STDIN_FILENO);
	dup2(origin_fd[1], STDOUT_FILENO);
	close(origin_fd[0]);
	close(origin_fd[1]);
}
