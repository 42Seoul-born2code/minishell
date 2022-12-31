#include "execute.h"
#include "utils.h"
#include "mini_signal.h"

int	exit_code;

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

static void	get_user_input(char *limiter)
{	
	int		fd;
	char	*input;
	char	*expand_result;
	pid_t	pid;

	change_heredoc_signal();
	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		fd = open(HEREDOC_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		while (exit_code != 1)
		{
			input = readline(HEREDOC_PROMPT);
			if (is_equal_to_limiter(input, limiter))
				break ;
			expand_result = expand_env_variable(input);
			ft_putstr_fd(expand_result, fd);
		}
		close(fd);
		exit(EXIT_SUCCESS);
	}
	else
	{
		waitpid(pid, NULL, 0);
		fd = open(HEREDOC_FILE, O_RDONLY, 0644);
		dup2(fd, STDIN_FILENO);
		close(fd);
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
		get_user_input(((t_token_node *)curr_node->next->content)->word);
		redirect_info.type = INFILE;
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
	char			**envp;
	t_list			*curr_node;
	t_token_node	*curr_token;

	save_origin_fd(origin_fd);
	cmd_path = NULL;
	// 자식 프로세스에서 시그널을 받았을 때 처리
	// 자식에서 ctrl-c exit();
	curr_node = token_list->head_node;
	redirect_info.file = NONE;
	while (curr_node != NULL)
	{
		// TODO: whitespace 인 경우에는 실행하지 않도록 처리
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
		}
		else if (is_redirection(curr_token) == TRUE)
		{
			redirect_info = process_redirection(curr_node);
		}
		curr_node = curr_node->next;
	}
	// TODO: 멀티 파이프인 경우에는 fork 를 사용해서 빌트인 함수를 실행시켜야 한다.
	if (redirect_info.file != NONE)
		close(redirect_info.file);
	change_signal();
	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		if (cmd_path == NULL)
			exit(EXIT_SUCCESS);
		envp = get_envp_in_list(env_list);
		execute_cmd(cmd_path, cmd_argv, envp);
	}
	else
	{
		// TODO: 자식 프로세스 반환값 전역 변수에 저장
		waitpid(pid, NULL, 0);
		unlink(HEREDOC_FILE);
		rollback_origin_fd(origin_fd);
	}
}
