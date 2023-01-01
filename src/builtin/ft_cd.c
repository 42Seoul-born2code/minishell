#include "builtin.h"
#include <dirent.h>

/*
	실제로 디렉토리를 이동하기 전에, 미리 모든 경로가 유효한지 탐색
*/
t_bool	is_valid_path(char **paths, t_env_list *env_list)
{
	int		idx;
	char	*curr_path;

	idx = 0;
	curr_path = NULL;
	while (paths[idx] != NULL)
	{
		if (ft_strcmp(paths[idx], ".") == 0)
			idx += 1;
		else if (ft_strcmp(paths[idx], "..") == 0)
		{
			if (curr_path != NULL)
				free(curr_path);
			curr_path = get_parent_directory(env_list);
			idx += 1;
		}
		else if (is_path_existed(&curr_path, paths, &idx) == FALSE)
		{
			free(curr_path);
			return (FALSE);
		}
	}
	return (TRUE);
}

/*
	change_directory 함수의 서브함수
*/
static char	*change_current_directory(char **paths, int *idx, t_env_list *env_list)
{
	char	*curr_path;
	char	*target_path;
	char	*buffer;

	if (ft_strcmp(paths[*idx], "..") == 0)
	{
		target_path = get_parent_directory(env_list);
		chdir(target_path);
		*idx += 1;
	}
	else
	{
		curr_path = getcwd(NULL, BUFSIZ);
		buffer = join_path(paths, idx);
		target_path = ft_strjoin(curr_path, buffer);
		if (chdir(target_path) == ERROR)
			chdir(buffer);
		free(buffer);
		free(curr_path);
	}
	return (target_path);
}

/*
	실제로 현재 디렉토리를 이동하는 함수
*/
void	change_directories(char **paths, t_env_list *env_list)
{
	int		idx;
	char	*target_path;

	idx = 0;
	replace_env_value(env_list, "OLDPWD", getcwd(NULL, BUFSIZ));
	while (paths[idx] != NULL)
	{
		if (ft_strcmp(paths[idx], ".") == 0)
			idx += 1;
		else
		{
			target_path = change_current_directory(paths, &idx, env_list);
			// TODO: cd /dev 실행 후 PWD 변수에 현재 경로 + dev 로 저장됨
			replace_env_value(env_list, "PWD", target_path);
			free(target_path);
		}
	}
}

int	ft_cd(char **argv, t_env_list *env_list)
{
	char	**paths;

	if (argv[1] == NULL)
	{
		if (is_env_existed(env_list, "HOME") == TRUE)
			return (move_to_env_path("HOME", env_list));
		return (print_error(HOME_IS_UNSET, NULL));
	}
	if (ft_strcmp(argv[1], "-") == 0 || ft_strcmp(argv[1], "--") == 0)
		return (move_to_env_path("OLDPWD", env_list));
	paths = ft_split(argv[1], '/');
	if (is_valid_path(paths, env_list) == FALSE)
	{
		free_all(paths);
		return (print_error(NOT_EXISTED, argv[1]));
	}
	change_directories(paths, env_list);
	free_all(paths);
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