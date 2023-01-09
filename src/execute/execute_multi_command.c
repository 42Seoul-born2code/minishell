/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multi_command.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:17:44 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/08 13:10:47 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execute.h"
/*
	리다이렉션은 없고 파이프만 들어온 경우
	- ls | cat
	- 파이프가 오기 전까지 command + argument 조합
	- 자식 생성 후 실행
	- 프로세스 카운트 + 1
*/

void	last_child_process(t_cmd_info *cmd_info, t_env_list *env_list, \
					int origin_fd[2], t_redirect redirect_info)
{
	pid_t	pid;

	change_signal();
	if (redirect_info.type != NORMAL && g_exit_code == EXIT_FAILURE)
		return ;
	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		if (redirect_info.type != OUTFILE)
			dup2(origin_fd[1], STDOUT_FILENO);
		if (is_builtin_function(cmd_info->cmd_name) == TRUE)
			exit(execute_builtin_function(cmd_info->cmd_name, \
					cmd_info->cmd_argv, env_list, MULTI_COMMAND));
		else
			execute_cmd(cmd_info->cmd_name, cmd_info->cmd_argv, \
						redirect_info, env_list);
	}
	else
	{
		waitpid(pid, &g_exit_code, 0);
		if (WIFSIGNALED(g_exit_code))
			g_exit_code = 128 + WTERMSIG(g_exit_code);
		else
			g_exit_code = WEXITSTATUS(g_exit_code);
	}
}

void	execute_multi_command(t_token *token_list, t_env_list *env_list)
{
	t_cmd_info		cmd_info;
	t_redirect		redirect_info;
	int				process_count;
	int				origin_fd[2];

	save_origin_fd(origin_fd);
	init_cmd_info(&cmd_info, INIT);
	init_redirect_info(&redirect_info);
	redirect_info.heredoc_file_num = 0;
	process_count = process_tokens(token_list->head_node, \
						&cmd_info, &redirect_info, env_list);
	last_child_process(&cmd_info, env_list, origin_fd, redirect_info);
	init_cmd_info(&cmd_info, FREE);
	rollback_origin_fd(origin_fd);
	while (process_count > 0)
	{
		if (wait(NULL) == ERROR)
			exit(EXIT_FAILURE);
		process_count -= 1;
	}
	delete_heredoc_file(redirect_info.heredoc_file_num);
}

/*

	1. 파이프 이전까지 command, argument 구분 및 redirection 처리
		- redirection 이 존재한다면 파이프로 넘기지말고 redirection 을 처리해야함
		- redirection 이 infile 처리인지 outfile 처리인지에 따라 파이프로 넘겨야 하는지가 결정됨
	2. 파이프를 만나면 파이프 연결
	3. 파이프 이전 명령어 실행
	4. 반복

*/

/*

	TEST CASES

	ls | cat
	=> ls 결과 출력

	ls > outfile
	< outfile cat | wc -l
	=> outfile 라인 수 출력

	ls > outfile | cat outfile
	=> 출력이 될 때도 있고 안될 때도 있음
*/
