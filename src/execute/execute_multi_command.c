#include "execute.h"

/*
	리다이렉션은 없고 파이프만 들어온 경우
	- ls | cat
	- 파이프가 오기 전까지 command + argument 조합
	- 자식 생성 후 실행
	- 프로세스 카운트 + 1
*/

// static void	execute_cmd(char *cmd_path, char *cmd_argv, char **envp)
// {
// 	if (execve(cmd_path, cmd_argv, envp) == ERROR)
// 	{
// 		print_error(COMMAND_NOT_FOUND, cmd_argv[READ]);
// 		free_double_arr(cmd_argv);
// 		free(cmd_path);
// 		exit(ERROR_CODE_COMMAND_NOT_FOUND);
// 	}
// }

// static int	child_process(char *cmd_path, char *cmd_argv, char **envp)
// {
// 	pid_t	pid;
// 	int		pipe_fd[2];

// 	if (pipe(pipe_fd) == ERROR)
// 		throw_error(PIPE_ERROR);
// 	pid = fork();
// 	if (pid == ERROR)
// 		throw_error(FORK_ERROR);
// 	if (pid == CHILD_PROCESS)
// 	{
// 		close(pipe_fd[READ]);
// 		dup2(pipe_fd[WRITE], STDOUT_FILENO);
// 		close(pipe_fd[WRITE]);
// 		execute_cmd(cmd_path, cmd_argv, envp);
// 	}
// 	else
// 	{
// 		close(pipe_fd[WRITE]);
// 		dup2(pipe_fd[READ], STDIN_FILENO);ffff
// 		close(pipe_fd[READ]);
// 		waitpid(pid, NULL, WNOHANG);
// 	}
// 	return (1);
// }

// void	last_child_process(char *cmd_path, char *cmd_argv, char **envp, int origin_stdout)
// {
// 	pid_t	pid;
// 	int		status;
// 	int		pipe_fd[2];

// 	if (pipe(pipe_fd) == ERROR)
// 		throw_error(PIPE_ERROR);
// 	pid = fork();
// 	if (pid == ERROR)
// 		throw_error(FORK_ERROR);
// 	if (pid == CHILD_PROCESS)
// 	{
// 		close(pipe_fd[READ]);
// 		dup2(origin_stdout, STDOUT_FILENO);
// 		close(pipe_fd[WRITE]);
// 		execute_cmd(cmd_path, cmd_argv, envp);
// 	}
// 	else
// 	{
// 		close(pipe_fd[WRITE]);
// 		dup2(pipe_fd[READ], STDIN_FILENO);
// 		close(pipe_fd[READ]);
// 		waitpid(pid, &status, 0);
// 	}
// }

// void	execute_multi_command(t_token *token_list, t_env_list env_list)
// {
// 	int				origin_stdout;
// 	int				process_count;
// 	char			*cmd_path;
// 	char			**cmd_argv;
// 	char			**envp;
// 	t_list			*curr_node;
// 	t_token_node	*curr_token;

// 	origin_stdout = dup(STDOUT_FILENO);
// 	process_count = 0;
// 	curr_node = token_list->head_node;
// 	while (curr_node != NULL)
// 	{
// 		curr_token = curr_node->content;
// 		if (curr_token->type == COMMAND)
// 		{
// 			cmd_path = find_cmd_path(curr_token->word);
// 			if (cmd_path == NULL)
// 			{
// 				printf("%s: command not found\n", curr_token->word);
// 			}
// 			cmd_argv = merge_arguments(curr_node);
// 			envp = get_envp_in_list(&env_list);
// 		}
// 		else if (curr_token->type == PIPE)
// 		{
// 			process_count += child_process(cmd_path, cmd_argv, envp);
// 		}
// 		curr_node = curr_node->next;
// 	}
// 	last_child_process(argc, argv, envp, origin_stdout);

// 	// 자식 프로세스 회수
// 	while (process_count > 0)
// 	{
// 		if (wait(NULL) == ERROR)
// 			exit(EXIT_FAILURE);
// 		else
// 			process_count -= 1;
// 	}
// }
