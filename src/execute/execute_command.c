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
			// 0. regular file 인지 확인
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
		// if (curr_token->type != ARGUMENT)
		// {
		// 	break ;
		// }
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

void	execute_command(t_token *token_list, t_env_list env_list)
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
		/* 
			- `filename` 을 현재 디렉토리에서 찾아본다.
			- 만약, 실행 가능하다면 `execve` 함수로 실행시킨다.
			- 현재 디렉토리에 없으면 `$PATH` 환경변수에 저장된 경로를 찾아가서 파일이 존재하는지 확인하고, 실행 권한이 있으면 `execve` 함수로 실행시킨다.
		*/
		if (curr_token->type == COMMAND)
		{
			// 1. 파일의 위치를 확인
			// 1-1. 현재 디렉토리
			// 1-2. $PATH
			cmd_path = find_cmd_path(curr_token->word);
			if (cmd_path == NULL)
				return ;
			cmd_argv = merge_arguments(curr_node);
			envp = get_envp_in_list(&env_list);
			break ;
			// 2. stat 으로 regular file 인지, directory 인지 판단
			// 3. 접근 권한(F_OK, X_OK)이 있는지 판단
			// 4. execve 실행
		}
		// else if (curr_token->type == ARGUMENT)
		// {
		// }
		curr_node = curr_node->next;
	}
	if (execve(cmd_path, cmd_argv, envp) == ERROR)
	{
		printf("실행 오류 발생\n");
	}
}
