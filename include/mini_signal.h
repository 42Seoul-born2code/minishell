/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_signal.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongkpa <jeongkpa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:18:58 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/08 15:59:56 by jeongkpa         ###   ########.fr       */
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

//termios.c
void	echoctl_on(void);
void	echoctl_off(void);

#endif
