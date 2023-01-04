#include "minishell.h"
#include "execute.h"
#include "utils.h"
#include "mini_signal.h"

void	throw_error(char *msg)
{
	print_error(msg, NULL);
	exit(EXIT_FAILURE);
}

char	*merge_word_list(t_word_list *word_list)
{
	char	*buffer;
	char	*result;
	t_list	*curr_word;
	t_list	*next_word;

	buffer = NULL;
	result = NULL;
	curr_word = word_list->head_node;
	while (curr_word != NULL)
	{
		next_word = curr_word->next;
		buffer = ft_strdup(result);
		if (result != NULL)
			free(result);
		result = ft_strjoin(buffer, (char *)curr_word->content);
		free(buffer);
		free(curr_word->content);
		free(curr_word);
		curr_word = next_word;
	}
	return (result);
}

char	*expand_env_variable(char *input)
{
	int				idx;
	int				start;
	int				word_length;
	char			*env_word;
	char			*buffer;
	char			*result;
	t_word_list		*word_list;

	idx = 0;
	word_list = malloc(sizeof(t_word_list));
	word_list->head_node = NULL;
	while (input[idx] != '\0')
	{
		if (input[idx] == '$')
		{
			idx += 1;
			start = idx;
			while (input[idx] != '\0')
			{
				if (is_valid_variable_rule(input[idx]) == FALSE)
					break ;
				if (is_operator(&input[idx]) == TRUE || \
					is_whitespace(input[idx]) == TRUE)
					break ;
				idx += 1;
			}
			if (start == idx)
				ft_lstadd_back(&word_list->head_node, \
						ft_lstnew(ft_strdup("$")));
			else
			{
				word_length = idx - start;
				env_word = malloc(sizeof(char) * (word_length + 1));
				ft_memcpy(env_word, &input[start], word_length);
				ft_lstadd_back(&word_list->head_node, \
						ft_lstnew(ft_strdup(getenv(env_word))));
				free(env_word);
			}
		}
		else
		{
			start = idx;
			while (input[idx] != '\0' && input[idx] != '$')
				idx += 1;
			word_length = idx - start;
			buffer = malloc(sizeof(char) * (word_length + 1));
			ft_memcpy(buffer, &input[start], word_length);
			ft_lstadd_back(&word_list->head_node, ft_lstnew(ft_strdup(buffer)));
			free(buffer);
		}
	}
	result = merge_word_list(word_list);
	free(word_list);
	return (result);
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

void	get_user_input(char *limiter, int heredoc_idx)
{	
	int		fd;
	char	*input;
	char	*expand_result;
	pid_t	pid;

	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		change_heredoc_signal();
		fd = get_heredoc_file_fd(heredoc_idx, WRITE_MODE);
		while (TRUE)
		{
			input = readline(HEREDOC_PROMPT);
			if (ft_strcmp(input, limiter) == 0)
				break ;
			expand_result = expand_env_variable(input);
			ft_putstr_fd(expand_result, fd);
			ft_putstr_fd("\n", fd);
		}
		close(fd);
		exit(EXIT_SUCCESS);
	}
	else
	{
		waitpid(pid, &g_exit_code, 0);
		g_exit_code = WEXITSTATUS(g_exit_code);
	}
}

void	process_redirection(t_list *curr_node, t_redirect *redirect_info)
{
	t_token_node	*curr_token;

	// << eof cat << foe cat
	if (redirect_info->file != NONE)
	{
		close(redirect_info->file);
		if (redirect_info->type == OUTFILE)
			close(STDOUT_FILENO);
		else if (redirect_info->type == INFILE)
			close(STDIN_FILENO);
	}
	curr_token = curr_node->content;
	if (curr_token->type == REDIR_RIGHT)
	{
		redirect_info->file = open_file(curr_node->next, WRITE_MODE);
		redirect_info->type = OUTFILE;
		dup2(redirect_info->file, STDOUT_FILENO);
	}
	else if (curr_token->type == REDIR_HEREDOC)
	{
		//TODO: heredoc file 이름 바꿔야함
		redirect_info->file = get_heredoc_file_fd(redirect_info->heredoc_file_num, READ_MODE);
		redirect_info->type = HEREDOC;
		dup2(redirect_info->file, STDIN_FILENO);
		redirect_info->heredoc_file_num += 1;
	}
	else if (curr_token->type == REDIR_APPEND)
	{
		redirect_info->file = open_file(curr_node->next, APPEND_MODE);
		redirect_info->type = OUTFILE;
		dup2(redirect_info->file, STDOUT_FILENO);
	}
	else if (curr_token->type == REDIR_LEFT)
	{
		redirect_info->file = open_file(curr_node->next, READ_MODE);
		redirect_info->type = INFILE;
		dup2(redirect_info->file, STDIN_FILENO);
	}
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
		// unlink(HEREDOC_FILE);
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
		// unlink(HEREDOC_FILE);
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
