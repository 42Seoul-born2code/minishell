#include "execute.h"

void	throw_error(char *msg)
{
	print_error(msg, NULL);
	exit(EXIT_FAILURE);
}

static int	is_equal_to_limiter(char *line, char *limiter)
{
	ssize_t	line_len;
	ssize_t	limiter_len;

	line_len = ft_strlen(line);
	limiter_len = ft_strlen(limiter);
	if (ft_strncmp(line, limiter, limiter_len) != 0)
		return (FALSE);
	if (line_len - 1 != limiter_len)
		return (FALSE);
	return (TRUE);
}

static void	get_user_input(char *limiter)
{	
	pid_t	pid;
	char	*input;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		throw_error(PIPE_ERROR);
	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		close(pipe_fd[READ]);
		while (TRUE)
		{
			ft_putstr_fd("heredoc> ", STDIN_FILENO);
			input = get_next_line(STDIN_FILENO);
			if (is_equal_to_limiter(input, limiter))
				exit(EXIT_SUCCESS);
			ft_putstr_fd(input, pipe_fd[WRITE]);
		}
		close(pipe_fd[WRITE]);
	}
	else
	{
		close(pipe_fd[WRITE]);
		printf("dup2: %d\n", dup2(pipe_fd[READ], STDIN_FILENO));
		waitpid(pid, NULL, 0);
		close(pipe_fd[READ]);
	}
}

void	execute_simple_command(t_token *token_list, t_env_list env_list)
{
	pid_t			pid;
	int				file;
	char			*cmd_path;
	char			**cmd_argv;
	char			**envp;
	t_list			*curr_node;
	t_token_node	*curr_token;

	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		curr_node = token_list->head_node;
		while (curr_node != NULL)
		{
			curr_token = curr_node->content;
			if (curr_token->type == COMMAND)
			{
				cmd_path = find_cmd_path(curr_token->word);
				if (cmd_path == NULL)
				{
					printf("%s: command not found\n", curr_token->word);
					exit(ERROR_CODE_COMMAND_NOT_FOUND);
				}
				cmd_argv = merge_arguments(curr_node);
				envp = get_envp_in_list(&env_list);
			}
			else if (curr_token->type == REDIR_RIGHT)
			{
				file = open_file(curr_node->next, WRITE_MODE);
				dup2(file, STDOUT_FILENO);
			}
			else if (curr_token->type == REDIR_APPEND)
			{
				file = open_file(curr_node->next, APPEND_MODE);
				dup2(file, STDOUT_FILENO);
			}
			else if (curr_token->type == REDIR_LEFT)
			{
				file = open_file(curr_node->next, READ_MODE);
				dup2(file, STDIN_FILENO);
			}
			else if (curr_token->type == REDIR_HEREDOC)
			{
				get_user_input(((t_token_node *)curr_node->next->content)->word);
			}
			curr_node = curr_node->next;
		}
		close(file);
		if (execve(cmd_path, cmd_argv, envp) == ERROR)
		{
			printf("execve error occured\n");
		}
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}
