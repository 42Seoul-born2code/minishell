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
		if (is_redirection(curr_token) == TRUE)
		{
			break ;
		}
		cmd_argv[idx] = ft_strdup(curr_token->word);
		idx += 1;
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
		if (is_redirection(curr_token) == TRUE || curr_token->type == PIPE)
			break ;
		argv_count += 1;
		curr_node = curr_node->next;
	}
	return (argv_count);
}

char	*find_cmd_path(char *cmd)
{
	int			i;
	char		*path;
	char		**path_env;
	char		*cmd_path;
	struct stat	buf;

	stat(cmd, &buf);
	if (S_ISDIR(buf.st_mode))
	{
		printf("%s is a directory\n", cmd);
		return (NULL);
	}
	i = 0;
	// 1. 현재 폴더에 위치하는지 확인
	if (access(cmd, F_OK | X_OK) == 0)
	{
		return (ft_strdup(cmd));
	}
	// 2. PATH 환경변수에 존재하는지 확인
	path_env = ft_split(getenv("PATH"), ':');
	if (path_env == NULL)
	{
		printf("환경 변수가 아리마셍.\n");
		return (NULL);
	}
	while (path_env[i] != NULL)
	{
		path = ft_strjoin(path_env[i], "/");
		cmd_path = ft_strjoin(path, cmd);
		// 존재하는 경우
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			return (cmd_path);
		}
		free(path);
		free(cmd_path);
		i += 1;
	}
	// 존재하지 않는 경우
	return (NULL);
}
