#include "minishell.h"
#include "execute.h"
/*
	리다이렉션은 없고 파이프만 들어온 경우
	- ls | cat
	- 파이프가 오기 전까지 command + argument 조합
	- 자식 생성 후 실행
	- 프로세스 카운트 + 1
*/

void	execute_cmd(char *cmd_path, char **cmd_argv, t_env_list *env_list)
{
	char	**envp;

	if (cmd_path == NULL && cmd_argv == NULL)
		exit(EXIT_SUCCESS);
	envp = get_envp_in_list(env_list);
	if (execve(cmd_path, cmd_argv, envp) == ERROR)
	{
		print_error(COMMAND_NOT_FOUND, cmd_argv[0]);
		free_all(cmd_argv);
		free_all(envp);
		free(cmd_path);
		// g_exit_code = ERROR_CODE_COMMAND_NOT_FOUND;
		exit(ERROR_CODE_COMMAND_NOT_FOUND);
	}
}

void	last_child_process(char *cmd_path, char **cmd_argv, t_env_list *env_list, int origin_fd[2], t_redirect redirect_info)
{
	pid_t	pid;

	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		if (redirect_info.type != OUTFILE)
			dup2(origin_fd[1], STDOUT_FILENO);
		if (is_builtin_function(cmd_path) == TRUE)
			exit(execute_builtin_function(cmd_path, cmd_argv, env_list));
		else
		{
			execute_cmd(cmd_path, cmd_argv, env_list);
		}
	}
	else
	{
		waitpid(pid, &g_exit_code, 0);
		if (WIFSIGNALED(g_exit_code))
			g_exit_code = 128 + WTERMSIG(g_exit_code);
		else
			g_exit_code = WEXITSTATUS(g_exit_code);
		close(STDIN_FILENO);
	}
}

static int	child_process(char *cmd_path, char **cmd_argv, t_env_list *env_list, t_redirect redirect_info)
{
	pid_t	pid;
	int		pipe_fd[2];

	if (redirect_info.type != OUTFILE)
		pipe(pipe_fd);
	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		if (redirect_info.type != OUTFILE)
		{
			close(pipe_fd[READ]);
			dup2(pipe_fd[WRITE], STDOUT_FILENO);
			close(pipe_fd[WRITE]);
		}
		if (is_builtin_function(cmd_path) == TRUE)
			exit(execute_builtin_function(cmd_path, cmd_argv, env_list));
		else if (redirect_info.file == NONE && redirect_info.type != NORMAL)
			exit(EXIT_FAILURE);
		else
			execute_cmd(cmd_path, cmd_argv, env_list);
	}
	else
	{
		if (redirect_info.type != OUTFILE)
		{
			close(pipe_fd[WRITE]);
			dup2(pipe_fd[READ], STDIN_FILENO);
			close(pipe_fd[READ]);
		}
		waitpid(pid, NULL, WNOHANG);
	}
	return (1);
}

void	execute_multi_command(t_token *token_list, t_env_list *env_list)
{
	t_redirect		redirect_info;
	int				origin_fd[2];
	int				process_count;
	char			*cmd_path;
	char			**cmd_argv;
	t_list			*curr_node;
	t_token_node	*curr_token;

	redirect_info.file = NONE;
	redirect_info.type = NORMAL;
	process_count = 0;
	curr_node = token_list->head_node;
	save_origin_fd(origin_fd);
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		if (curr_token->type == COMMAND)
		{
			if (is_builtin_function(curr_token->word) == TRUE)
				cmd_path = ft_strdup(curr_token->word);
			else
				cmd_path = find_cmd_path(curr_token->word, env_list);
			if (cmd_path == NULL)
			{
				printf("%s: command not found\n", curr_token->word);
			}
			cmd_argv = merge_arguments(curr_node);
		}
		else if (is_redirection(curr_token) == TRUE)
		{
			redirect_info = process_redirection(curr_node);
		}
		else if (curr_token->type == PIPE)
		{
			process_count += child_process(cmd_path, cmd_argv, env_list, redirect_info);
			free(cmd_path);
			free_all(cmd_argv);
			redirect_info.file = NONE;
			redirect_info.type = NORMAL;
		}
		curr_node = curr_node->next;
	}
	last_child_process(cmd_path, cmd_argv, env_list, origin_fd, redirect_info);
	free(cmd_path);
	free_all(cmd_argv);
	rollback_origin_fd(origin_fd);
	while (process_count > 0)
	{
		if (wait(NULL) == ERROR)
			exit(EXIT_FAILURE);
		process_count -= 1;
	}
	unlink(HEREDOC_FILE);
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
