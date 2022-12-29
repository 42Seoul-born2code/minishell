#include "builtin.h"
#include <dirent.h>

/*
	매개변수로 전달된 환경변수의 경로로 현재 위치를 변경
	- env_path: 이동할 환경변수
	- env_list: 환경변수 리스트
*/
int	move_to_env_path(char *env_path, t_env_list *env_list)
{
	char	*curr_path;
	char	*target_path;

	if (is_env_existed(env_list, env_path) == FALSE)
		return (EXIT_FAILURE);
	curr_path = getcwd(NULL, BUFSIZ);
	target_path = get_env_value(env_list, env_path);
	if (chdir(target_path) == ERROR)
	{
		free(curr_path);
		free(target_path);
		return (EXIT_FAILURE);
	}
	replace_env_value(env_list, "OLDPWD", curr_path);
	replace_env_value(env_list, "PWD", target_path);
	free(curr_path);
	return (EXIT_SUCCESS);
}

char	*get_parent_directory(void)
{
	size_t	len;
	char	*curr_path;
	char	*parent_dir;

	curr_path = getcwd(NULL, BUFSIZ);
	len = ft_strlen(curr_path);
	while (curr_path[len] != '/')
		len -= 1;
	parent_dir = ft_substr(curr_path, 0, len);
	free(curr_path);
	return (parent_dir);
}

char	*join_path(char **paths, int *idx)
{
	char	*buffer;
	char	*slashed_path;
	char	*joined_path;

	buffer = NULL;
	joined_path = NULL;
	while (paths[*idx] != NULL)
	{
		if (ft_strcmp(paths[*idx], ".") == 0 || \
			ft_strcmp(paths[*idx], "..") == 0)
			break ;
		buffer = ft_strdup(joined_path);
		slashed_path = ft_strjoin(buffer, "/");
		if (joined_path != NULL)
			free(joined_path);
		joined_path = ft_strjoin(slashed_path, paths[*idx]);
		free(buffer);
		free(slashed_path);
		*idx += 1;
	}
	return (joined_path);
}

t_bool	move_to_directory(char **curr_path, char **paths, int *idx)
{
	DIR		*dir;
	DIR		*abs_dir;
	char	*joined_path;
	char	*abs_path;

	if (*curr_path == NULL)
		*curr_path = getcwd(NULL, BUFSIZ);
	abs_path = join_path(paths, idx);
	abs_dir = opendir(abs_path);
	if (abs_dir != NULL)
	{
		closedir(abs_dir);
		free(*curr_path);
		*curr_path = abs_path;
	}
	else
	{
		joined_path = ft_strjoin(*curr_path, abs_path);
		dir = opendir(joined_path);
		if (dir == NULL)
			return (FALSE);
		closedir(dir);
		free(*curr_path);
		*curr_path = joined_path;
	}
	return (TRUE);
}

t_bool	is_valid_path(char *path)
{
	int		idx;
	char	*curr_path;
	char	*parent_dir;
	char	**paths;

	idx = 0;
	curr_path = NULL;
	paths = ft_split(path, '/');
	while (paths[idx] != NULL)
	{
		if (ft_strcmp(paths[idx], ".") == 0)
		{
			idx += 1;
			continue ;
		}
		else if (ft_strcmp(paths[idx], "..") == 0)
		{
			if (curr_path == NULL)
				curr_path = getcwd(NULL, BUFSIZ);
			parent_dir = get_parent_directory();
			free(curr_path);
			curr_path = parent_dir;
			idx += 1;
		}
		else if (move_to_directory(&curr_path, paths, &idx) == FALSE)
		{
			free_all(paths);
			return (FALSE);
		}
	}
	free_all(paths);
	return (TRUE);
}

void	change_directories(char *path, t_env_list *env_list)
{
	int		idx;
	char	*curr_path;
	char	*target_path;
	char	*buffer;
	char	**paths;

	idx = 0;
	replace_env_value(env_list, "OLDPWD", getcwd(NULL, BUFSIZ));
	paths = ft_split(path, '/');
	while (paths[idx] != NULL)
	{
		if (ft_strcmp(paths[idx], ".") == 0)
		{
			idx += 1;
			continue ;
		}
		if (ft_strcmp(paths[idx], "..") == 0)
		{
			target_path = get_parent_directory();
			chdir(target_path);
			idx += 1;
		}
		else
		{
			curr_path = getcwd(NULL, BUFSIZ);
			buffer = join_path(paths, &idx);
			target_path = ft_strjoin(curr_path, buffer);
			if (chdir(target_path) == ERROR)
				chdir(buffer);
			free(buffer);
			free(curr_path);
		}
		replace_env_value(env_list, "PWD", target_path);
		free(target_path);
	}
	free_all(paths);
}

int	ft_cd(char **argv, t_env_list *env_list)
{
	if (argv[1] == NULL)
	{
		if (is_env_existed(env_list, "HOME") == TRUE)
			return (move_to_env_path("HOME", env_list));
		return (print_error(HOME_IS_UNSET, NULL));
	}
	if (ft_strcmp(argv[1], "-") == 0 || ft_strcmp(argv[1], "--") == 0)
		return (move_to_env_path("OLDPWD", env_list));
	if (is_valid_path(argv[1]) == FALSE)
		return (print_error(NOT_EXISTED, argv[1]));
	change_directories(argv[1], env_list);
	return (EXIT_SUCCESS);
}

/*

	TEST CASES

	cd include/
	echo $?
	> 0

	// 경로가 여러 개 있어도 가장 첫 번째 경로만 인식
	cd include/ libft/
	echo $?
	> 0

	cd not_existed_path/ libft/
	> cd: not_existed_path/: No such file or directory
	echo $?
	> 1

	cd
	> HOME 변수에 설정된 위치로 이동

	unset HOME
	cd
	> cd: HOME not set

	cd .
	> 현재 디렉토리에 그대로 있음
	cd ./
	> 현재 디렉토리에 그대로 있음
	cd ./././
	> 현재 디렉토리에 그대로 있음
	
	cd ..
	> 상위 디렉토리로 이동
	cd ../
	> 상위 디렉토리로 이동
	cd ..///
	> 상위 디렉토리로 이동
	cd ../minishell/..
	> 상위 디렉토리로 이동

	cd ../include
	> 상위 디렉토리의 include 디렉토리로 이동

	cd .././../
	> 상위 -> 상위 디렉토리로 이동

	cd /Users
	> Users 폴더로 이동

	cd .././42born2code

	cd ../
	> 상위 디렉토리 이동
	cd -
	> 이전 디렉토리 이동
	cd -
	> 다시 한번 이전 디렉토리 이동

	cd ../minishell/src/./../

*/

/*

	1. 인자가 유효한지 검사한다.
		- argv[1]이 존재하는 경로인가?
		- 상대 경로라면 . ./ .. ../ 로 시작하는가?
	
	2. '/' 를 기준으로 ft_split 수행
	
	3. getcwd 로 현재 위치를 받아온다.
		- . == getcwd 와 동일
		- .. == getcwd 에서 맨 마지막 경로만 제거

*/