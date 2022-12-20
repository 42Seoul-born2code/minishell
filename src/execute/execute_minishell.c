#include "execute.h"
#include <sys/stat.h>

void	print_token_word(t_token *token_list)
{
	t_list			*curr_node;
	t_token_node	*curr_token;

	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		printf("curr_token->word: %s\n", curr_token->word);
		curr_node = curr_node->next;
	}
}

void	free_list_nodes(t_token *lst)
{
	t_list				*curr_list;
	t_list				*next_list;
	t_token_node	*curr_node;

	curr_list = lst->head_node;
	while (curr_list != NULL)
	{
		next_list = curr_list->next;
		curr_node = curr_list->content;
		free(curr_node->word);
		free(curr_list);
		curr_list = next_list;
	}
	lst->head_node = NULL;
}

static char	*find_cmd_path(char *cmd)
{
	int		i;
	char	*path;
	char	**path_env;
	char	*cmd_path;
	// struct stat buf;

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
		// TODO 
		// execve 함수에 들어갔을 때 디렉토리를 실행하려고 하면
		// "___ is a directory" 라는 에러를 발생시키는지 확인하기
		// stat(cmd, &buf);
		// if (!S_ISREG(buf.st_mode))
		// {
		// 	printf("%s is a directory\n");
		// 	return (NULL);
		// }
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

void	execute_command(t_token *token_list)
{
	char			*cmd_path;
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
			// 2. stat 으로 regular file 인지, directory 인지 판단
			// 3. 접근 권한(F_OK, X_OK)이 있는지 판단
			// 4. execve 실행
		}
		// TODO argument 들은 char **로 포함하기
		// 리스트를 순회하면서 파이프가 나오기 전까지의 ARG 갯수 CNT
		// 그리고 나서 더블 포인터 malloc
		// strdup
		// cmd arg arg arg 
		else if (curr_token->type == ARGUMENT)
		{

		}
		curr_node = curr_node->next;
	}
}

void	execute_minishell(t_env_list env)
{
	char	*line;
	t_token	*token_list;

	(void)env;
	token_list = malloc(sizeof(t_token));
	token_list->head_node = NULL;
	while (TRUE)
	{
		line = readline(PROMPT);
		if (!line)
			break ;
		tokenize_line(line, token_list);
		parsing(token_list);
		syntax_analysis(token_list);
		expansion(token_list);
		quote_removal(token_list);
		print_token_word(token_list);
		// set_redirection(token_list);
		execute_command(token_list);
		free_list_nodes(token_list);
		add_history(line);
		free(line);
	}
}

// [cat] [Makefile]
// execve(cmd_path, cmd_argv, envp);

// cat Makefile > outfile
