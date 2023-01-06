/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_signal.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:18:58 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:19:52 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SIGNAL_H
# define MINI_SIGNAL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>

void	init_signal(void);
void	change_signal(void);
void	change_heredoc_signal_parent(void);
void	change_heredoc_signal_child(void);
#endif
