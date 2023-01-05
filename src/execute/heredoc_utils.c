#include "minishell.h"
#include "execute.h"

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

void	get_user_input(char *limiter, int heredoc_idx)
{	
	int		fd;
	char	*input;
	char	*expand_result;
	pid_t	pid;

	change_heredoc_signal();
	pid = fork();
	if (pid == CHILD_PROCESS)
	{
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

void	check_heredoc(t_token *token_list)
{
	t_list			*curr_node;
	t_token_node	*limiter_node;
	t_token_node	*curr_token;
	int				idx;

	idx = 0;
	curr_node = token_list->head_node;
	g_exit_code = 0;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		if (curr_token->type == REDIR_HEREDOC)
		{
			if (WIFSIGNALED(g_exit_code))
				break;
			limiter_node = curr_node->next->content;
			get_user_input(limiter_node->word, idx);
			idx += 1;
		}
		curr_node = curr_node->next;
	}
}

void	delete_heredoc_file(int heredoc_file_num)
{
	char	*heredoc_file_name;

	while (heredoc_file_num > 0)
	{
		heredoc_file_num -= 1;
		heredoc_file_name = ft_strjoin(HEREDOC_FILE, ft_itoa(heredoc_file_num));
		unlink (heredoc_file_name);
	}
}
