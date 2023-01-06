/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:17:55 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:18:03 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "mini_signal.h"

static void	handle_sigint_change_c(int signum)
{
	if (signum != SIGINT)
		return ;
	exit(1);
}

void	change_heredoc_signal_child(void)
{
	signal(SIGINT, handle_sigint_change_c);
}

static void	handle_sigint_change_p(int signum)
{
	if (signum != SIGINT)
		return ;
	printf("\n");
}

void	change_heredoc_signal_parent(void)
{
	signal(SIGINT, handle_sigint_change_p);
}
