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

void	get_user_input(char *limiter)
{	
	int		fd;
	char	*input;
	char	*expand_result;
	pid_t	pid;

	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		change_heredoc_signal();
		fd = open(HEREDOC_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0644);
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

t_redirect	process_redirection(t_list *curr_node)
{
	t_redirect		redirect_info;
	t_token_node	*curr_token;

	redirect_info.file = NONE;
	curr_token = curr_node->content;
	if (curr_token->type == REDIR_RIGHT)
	{
		redirect_info.file = open_file(curr_node->next, WRITE_MODE);
		redirect_info.type = OUTFILE;
		dup2(redirect_info.file, STDOUT_FILENO);
	}
	else if (curr_token->type == REDIR_HEREDOC)
	{
		redirect_info.file = open(HEREDOC_FILE, O_RDONLY, 0644);
		redirect_info.type = HEREDOC;
		dup2(redirect_info.file, STDIN_FILENO);
	}
	else if (curr_token->type == REDIR_APPEND)
	{
		redirect_info.file = open_file(curr_node->next, APPEND_MODE);
		redirect_info.type = OUTFILE;
		dup2(redirect_info.file, STDOUT_FILENO);
	}
	else if (curr_token->type == REDIR_LEFT)
	{
		redirect_info.file = open_file(curr_node->next, READ_MODE);
		redirect_info.type = INFILE;
		dup2(redirect_info.file, STDIN_FILENO);
	}
	return (redirect_info);
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
	curr_node = token_list->head_node;
	redirect_info.file = NONE;
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
			redirect_info = process_redirection(curr_node);
		}
		curr_node = curr_node->next;
	}
	if (redirect_info.file != NONE)
		close(redirect_info.file);
	if (g_exit_code != 0 && redirect_info.type == HEREDOC)
	{
		unlink(HEREDOC_FILE);
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
		g_exit_code = WEXITSTATUS(g_exit_code);
		unlink(HEREDOC_FILE);
		rollback_origin_fd(origin_fd);
		free(cmd_path);
		free_all(cmd_argv);
	}
}
