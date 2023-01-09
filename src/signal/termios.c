/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongkpa <jeongkpa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 18:17:55 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/08 18:17:56 by jeongkpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_signal.h"
#include <termios.h>

void	echoctl_on(void)
{
	struct termios	attr;

	if (isatty(STDIN_FILENO))
	{
		tcgetattr(STDIN_FILENO, &attr);
		attr.c_lflag = attr.c_lflag | ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSANOW, &attr);
	}
	else if (isatty(STDOUT_FILENO))
	{
		tcgetattr(STDOUT_FILENO, &attr);
		attr.c_lflag = attr.c_lflag | ECHOCTL;
		tcsetattr(STDOUT_FILENO, TCSANOW, &attr);
	}
	else if (isatty(STDERR_FILENO))
	{
		tcgetattr(STDERR_FILENO, &attr);
		attr.c_lflag = attr.c_lflag | ECHOCTL;
		tcsetattr(STDERR_FILENO, TCSANOW, &attr);
	}
}

void	echoctl_off(void)
{
	struct termios	attr;

	if (isatty(STDIN_FILENO))
	{
		tcgetattr(STDIN_FILENO, &attr);
		attr.c_lflag = attr.c_lflag & ~ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSANOW, &attr);
	}
	else if (isatty(STDOUT_FILENO))
	{
		tcgetattr(STDOUT_FILENO, &attr);
		attr.c_lflag = attr.c_lflag & ~ECHOCTL;
		tcsetattr(STDOUT_FILENO, TCSANOW, &attr);
	}
	else if (isatty(STDERR_FILENO))
	{
		tcgetattr(STDERR_FILENO, &attr);
		attr.c_lflag = attr.c_lflag & ~ECHOCTL;
		tcsetattr(STDERR_FILENO, TCSANOW, &attr);
	}
}
