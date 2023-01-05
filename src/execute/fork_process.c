#include "minishell.h"
#include "execute.h"
#include "utils.h"
#include "mini_signal.h"

void	throw_error(char *msg)
{
	print_error(msg, NULL);
	exit(EXIT_FAILURE);
}

int	get_heredoc_file_fd(int heredoc_idx, int mode)
{
	int		fd;
	char	*heredoc_file_name;

	fd = NONE;
	heredoc_file_name = ft_strjoin(HEREDOC_FILE, ft_itoa(heredoc_idx));
	if (mode == READ_MODE)
		fd = open(heredoc_file_name, O_RDONLY, 0644);
	else if (mode == WRITE_MODE)
		fd = open(heredoc_file_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	free(heredoc_file_name);
	return (fd);
}

void	fork_process(t_token *token_list, t_env_list *env_list)
{
	int				origin_fd[2];
	pid_t			pid;
	t_redirect		redirect_info;
	char			*cmd_path;
	char			**cmd_argv;
	t_list			*curr_node;
	t_token_node	*curr_token;

	change_signal();
	save_origin_fd(origin_fd);
	cmd_path = NULL;
	cmd_argv = NULL;
	curr_node = token_list->head_node;
	redirect_info.file = NONE;
	redirect_info.type = NORMAL;
	redirect_info.heredoc_file_num = 0;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		if (curr_token->type == COMMAND)
		{
			cmd_path = find_cmd_path(curr_token->word, env_list);
			cmd_argv = merge_arguments(curr_node);
		}
		else if (is_redirection(curr_token) == TRUE)
		{
			process_redirection(curr_node, &redirect_info);
		}
		curr_node = curr_node->next;
	}
	if (redirect_info.file != NONE)
		close(redirect_info.file);
	if (g_exit_code != 0 && redirect_info.type == HEREDOC)
	{
		rollback_origin_fd(origin_fd);
		return ;
	}
	if (redirect_info.file == NONE && redirect_info.type != NORMAL)
	{
		g_exit_code = 1;
		if (cmd_path != NULL)
			free(cmd_path);
		if (cmd_argv != NULL)
			free_all(cmd_argv);
		rollback_origin_fd(origin_fd);
		return ;
	}
	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		execute_cmd(cmd_path, cmd_argv, env_list);
	}
	else
	{
		waitpid(pid, &g_exit_code, 0);
		if (WIFSIGNALED(g_exit_code))
			g_exit_code = 128 + WTERMSIG(g_exit_code);
		else
			g_exit_code = WEXITSTATUS(g_exit_code);
		unlink(HEREDOC_FILE);
		rollback_origin_fd(origin_fd);
		if (cmd_path != NULL)
			free(cmd_path);
		if (cmd_argv != NULL)
			free_all(cmd_argv);
		delete_heredoc_file(redirect_info.heredoc_file_num);
	}
}
