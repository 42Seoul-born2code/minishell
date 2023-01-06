#include "builtin.h"
#include <dirent.h>

static t_bool	is_dir_accessible(char *path)
{
	DIR	*p_dir;

	p_dir = opendir(path);
	if (p_dir == NULL)
		return (FALSE);
	else
	{
		free(p_dir);
		return (TRUE);
	}
}

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
		return (print_error(VARIABLE_IS_UNSET, env_path));
	curr_path = ft_strdup(get_env_value(env_list, "PWD"));
	target_path = get_env_value(env_list, env_path);
	if (chdir(target_path) == ERROR)
	{
		return (print_error(VARIABLE_IS_UNSET, env_path));
	}
	replace_env_value(env_list, "PWD", target_path);
	replace_env_value(env_list, "OLDPWD", curr_path);
	free(curr_path);
	return (EXIT_SUCCESS);
}

t_bool	is_absolute_path_existed(char *path, char *old_pwd, \
					char **paths, t_env_list *env_list)
{
	struct stat	buf;
	char		*target_path;

	target_path = ft_strjoin("/", path);
	stat(target_path, &buf);
	if (!S_ISDIR(buf.st_mode))
	{
		print_error(NOT_EXISTED, target_path);
		return (FALSE);
	}
	if (is_dir_accessible(target_path) == FALSE)
	{
		print_error(PERMISSON_DENIED, path);
		return (FALSE);
	}
	if (chdir(target_path) == ERROR)
	{
		replace_env_value(env_list, "PWD", old_pwd);
		free(old_pwd);
		free_all(paths);
		free(target_path);
		return (print_error(NOT_EXISTED, path));
	}
	free(target_path);
	return (TRUE);
}

/*
	실제로 현재 디렉토리를 변경하는 함수
	1. 폴더가 존재하는지 먼저 확인: stat 이나 lstat 으로 먼저 확인
	2. 접근 가능한지 확인: opendir 로 확인
	3. 상대 경로 -> 절대 경로 순서대로 확인: 순차적으로 확인
*/
int	change_directories(char *argv, t_env_list *env_list)
{
	int		idx;
	char	*old_pwd;
	char	*curr_path;
	char	**paths;
	int		result;
	struct stat	buf;

	idx = -1;
	result = EXIT_SUCCESS;
	paths = ft_split(argv, '/');
	old_pwd = getcwd(NULL, BUFSIZ);
	while (paths[++idx] != NULL && result == EXIT_SUCCESS)
	{
		stat(paths[idx], &buf);
		// 1. 폴더가 존재할 때
		if (S_ISDIR(buf.st_mode))
		{
			// 2. 폴더가 접근 가능할 때
			if (is_dir_accessible(paths[idx]) == TRUE)
			{
				// 3. 상대 경로로 먼저 접근
				if (chdir(paths[idx]) == ERROR)
				{
					// 4. 절대 경로로 접근
					if (is_absolute_path_existed(paths[idx], old_pwd, paths, env_list) == FALSE)
					{
						result = EXIT_FAILURE;
					}
				}
			}
			else
			{
				result = print_error(PERMISSON_DENIED, argv);
			}
		}
		else
		{
			if (is_absolute_path_existed(paths[idx], old_pwd, paths, env_list) == FALSE)
			{
				result = EXIT_FAILURE;
			}
		}
		curr_path = getcwd(NULL, BUFSIZ);
		replace_env_value(env_list, "PWD", curr_path);
		free(curr_path);
	}
	replace_env_value(env_list, "OLDPWD", old_pwd);
	free(old_pwd);
	free_all(paths);
	return (result);
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
	return (change_directories(argv[1], env_list));
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