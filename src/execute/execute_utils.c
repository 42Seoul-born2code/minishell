#include "execute.h"

t_bool	is_redirection(t_token_node *curr_token)
{
	t_meta	type;

	type = curr_token->type;
	if (type == REDIR_APPEND || type == REDIR_HEREDOC || \
		type == REDIR_RIGHT || type == REDIR_LEFT)
		return (TRUE);
	return (FALSE);
}

char	**merge_arguments(t_list *curr_node)
{
	int				idx;
	int				argv_count;
	char			**cmd_argv;
	t_token_node	*curr_token;

	idx = 0;
	argv_count = count_argv(curr_node);
	cmd_argv = malloc((sizeof(char *)) * (argv_count + 1));
	while (idx < argv_count)
	{
		curr_token = curr_node->content;
		if (curr_token->type == PIPE)
			break ;
		if (is_redirection(curr_token) == TRUE && curr_node->next != NULL)
			curr_node = curr_node->next;
		else if (curr_token->type == ARGUMENT || curr_token->type == COMMAND)
		{
			// FIXME: word 가 NULL 인 경우 ft_strdup 이 NULL 을 반환
			// 그래서 다음 문자열이 있음에도 제대로 출력을 못함
			cmd_argv[idx] = ft_strdup(curr_token->word);
			idx += 1;
		}
		curr_node = curr_node->next;
	}
	cmd_argv[argv_count] = NULL;
	return (cmd_argv);
}

int	count_argv(t_list *curr_node)
{
	int				argv_count;
	t_token_node	*curr_token;

	argv_count = 0;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		if (curr_token->type == PIPE)
			break ;
		if (is_redirection(curr_token) == TRUE && curr_node->next != NULL)
			curr_node = curr_node->next;
		else if (curr_token->type == ARGUMENT || curr_token->type == COMMAND)
			argv_count += 1;
		curr_node = curr_node->next;
	}
	return (argv_count);
}

static char	**get_path_env(char *str)
{
	if (str == NULL)
		return NULL;
	else
		return (ft_split(str, ':'));	
}

char	*find_cmd_path(char *cmd, t_env_list *env_list)
{
	int			idx;
	char		*path;
	char		**path_env;
	char		*cmd_path;
	struct stat	buf;

	stat(cmd, &buf);
	if (S_ISDIR(buf.st_mode))
	{
		print_error(cmd, "is a directory\n");
		return (NULL);
	}
	idx = 0;
	// 1. 현재 폴더에 위치하는지 확인
	if (access(cmd, F_OK | X_OK) == 0)
	{
		return (ft_strdup(cmd));
	}
	// 2. PATH 환경변수에 존재하는지 확인
	path_env = get_path_env(get_env_value(env_list, "PATH"));
	if (path_env == NULL)
		return (NULL);
	while (path_env[idx] != NULL)
	{
		path = ft_strjoin(path_env[idx], "/");
		cmd_path = ft_strjoin(path, cmd);
		free(path);
		// 존재하는 경우
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			free_all(path_env);
			return (cmd_path);
		}
		free(cmd_path);
		idx += 1;
	}
	free_all(path_env);
	// 존재하지 않는 경우
	return (NULL);
}
