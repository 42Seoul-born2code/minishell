#include "builtin.h"
#include <dirent.h>

int	move_to_env_path(char *env_path, t_env_list *env_list)
{
	char	*curr_path;
	char	*target_path;

	if (is_env_existed(env_list, env_path) == FALSE)
		return (EXIT_FAILURE);
	target_path = get_env_value(env_list, env_path);
	if (chdir(target_path) == ERROR)
		return (EXIT_FAILURE);
	curr_path = getcwd(NULL, BUFSIZ);
	replace_env_value(env_list, "OLDPWD", curr_path);
	replace_env_value(env_list, "PWD", target_path);
	free(curr_path);
	free(target_path);
	return (EXIT_SUCCESS);
}

char	*join_path(char **paths, int *idx)
{
	int		sub_idx;
	char	*buffer;
	char	*slashed_path;
	char	*joined_path;

	sub_idx = 0;
	buffer = NULL;
	joined_path = NULL;
	while (paths[sub_idx] != NULL)
	{
		if (ft_strcmp(paths[sub_idx], ".") == 0 || \
			ft_strcmp(paths[sub_idx], "..") == 0)
			break ;
		buffer = ft_strdup(joined_path);
		slashed_path = ft_strjoin(buffer, "/");
		if (joined_path != NULL)
			free(joined_path);
		joined_path = ft_strjoin(slashed_path, paths[sub_idx]);
		free(buffer);
		free(slashed_path);
		sub_idx += 1;
		*idx += 1;
	}
	return (joined_path);
}

char	*get_parent_directory(char *path)
{
	size_t	len;
	char	*parent_dir;

	len = ft_strlen(path);
	while (path[len] != '/')
		len -= 1;
	parent_dir = ft_substr(path, 0, len);
	return (parent_dir);
}

t_bool	is_valid_path(char *path)
{
	int		idx;
	char	*curr_path;
	char	*parent_dir;
	char	*buffer;
	char	*joined_path;
	char	**paths;
	DIR		*dir;
	DIR		*abs_dir;

	idx = 0;
	curr_path = NULL;
	buffer = NULL;
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
			parent_dir = get_parent_directory(curr_path);
			free(curr_path);
			curr_path = parent_dir;
			idx += 1;
		}
		else
		{
			if (curr_path == NULL)
				curr_path = getcwd(NULL, BUFSIZ);
			buffer = join_path(&paths[idx], &idx);
			joined_path = ft_strjoin(curr_path, buffer);
			dir = opendir(joined_path);
			abs_dir = opendir(buffer);
			free(curr_path);
			if (dir == NULL && abs_dir == NULL)
				return (FALSE);
			if (dir != NULL)
			{
				closedir(dir);
				free(buffer);
				curr_path = joined_path;
			}
			else
			{
				closedir(abs_dir);
				free(joined_path);
				curr_path = buffer;
			}
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
	curr_path = getcwd(NULL, BUFSIZ);
	replace_env_value(env_list, "OLDPWD", curr_path);
	paths = ft_split(path, '/');
	while (paths[idx] != NULL)
	{
		if (ft_strcmp(paths[idx], ".") == 0)
		{
			idx += 1;
			continue ;
		}
		if (curr_path != NULL)
			free(curr_path);
		curr_path = getcwd(NULL, BUFSIZ);
		if (ft_strcmp(paths[idx], "..") == 0)
		{
			target_path = get_parent_directory(curr_path);
			chdir(target_path);
			idx += 1;
		}
		else
		{
			buffer = join_path(&paths[idx], &idx);
			target_path = ft_strjoin(curr_path, buffer);
			if (chdir(target_path) == ERROR)
				chdir(buffer);
			free(buffer);
		}
		replace_env_value(env_list, "PWD", target_path);
		free(curr_path);
		free(target_path);
		curr_path = NULL;
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
	> 다시 한번 이전 디렉토리 이동 (이 부분이 안됨)

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