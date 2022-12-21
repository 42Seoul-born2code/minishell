#include "execute.h"

static char	*find_cmd_path(char *cmd)
{
	int		i;
	char	*path;
	char	**path_env;
	char	*cmd_path;
	struct stat buf;

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
		printf("cmd: %s\n", cmd);
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
			printf("cmd_path: %s\n", cmd_path);
			return (cmd_path);
		}
		free(path);
		free(cmd_path);
		i += 1;
	}
	// 존재하지 않는 경우
	return (NULL);
}
static int	count_argv(t_list *curr_node)
{
	int	            argv_count;
	t_token_node	*curr_token;
	
	argv_count = 0;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		argv_count += 1;
		curr_node = curr_node->next;
	}
	return (argv_count);
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
		cmd_argv[idx] = ft_strdup(curr_token->word);
		idx += 1;
		curr_node = curr_node->next;
	}
	cmd_argv[argv_count] = NULL;
	return (cmd_argv);
}

e_command_type	get_execute_type(t_token *token_list)
{
	t_list			*curr_node;
	t_token_node	*curr_token;

	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		if (curr_token->type == PIPE)
			return (MULTI_COMMAND);
		curr_node = curr_node->next;
	}
	return (SIMPLE_COMMAND);
}

void	fork_command(char *cmd_path, char **cmd_argv, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		if (execve(cmd_path, cmd_argv, envp) == ERROR)
		{
			// printf("execve error occured\n");
		}
	}
}

void	execute_simple_command(t_token *token_list, t_env_list env_list)
{
	char			*cmd_path;
	char			**cmd_argv;
	char			**envp;
	t_list			*curr_node;
	t_token_node	*curr_token;

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
			}
			cmd_argv = merge_arguments(curr_node);
			envp = get_envp_in_list(&env_list);
			// 자식 프로세스로 실행
			fork_command(cmd_path, cmd_argv, envp);
			break ;
		}
		curr_node = curr_node->next;
	}

}

void	execute_command(t_token *token_list, t_env_list env_list)
{
	t_list			*curr_node;

	// TODO
	// 1. 파이프가 존재하지 않는 simple command 는 fork 를 수행해서 실행한다.
	// 2. 파이프가 존재하는 multi command 는 파이프 생성 후 fork 를 실행한다.
	curr_node = token_list->head_node;
	if (get_execute_type(token_list) == SIMPLE_COMMAND)
	{
		execute_simple_command(token_list, env_list);
	}
	else
	{
		// execute_multi_command();
	}
}

// ls | cat 