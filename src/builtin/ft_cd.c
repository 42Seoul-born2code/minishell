#include "builtin.h"

int	move_to_path(char *path, char *curr_path, t_env_list *env_list)
{
	char	*target_path;

	if (is_env_existed(env_list, path) == FALSE)
		return (EXIT_FAILURE);
	target_path = get_env_value(env_list, path);
	if (chdir(target_path) == ERROR)
		return (EXIT_FAILURE);
	replace_env_value(env_list, "OLDPWD", curr_path);
	replace_env_value(env_list, "PWD", target_path);
	return (EXIT_SUCCESS);
}

// t_bool	is_valid_path(char *path)
// {
// 	FILE	*dir;

// 	dir = opendir(path);
// 	// if ()
// 	return (FALSE);
// }

char	*expand_relative_path(char *path, char *curr_path)
{
	char	*buffer;
	char	*replaced_path;

	if (ft_strncmp(path, "./", 2) == 0)
		buffer = curr_path;
	else ()
}

int	ft_cd(char **argv, t_env_list *env_list)
{
	char	*curr_path;
	char	*target_path;

	curr_path = getcwd(NULL, BUFSIZ);
	if (argv[1] == NULL)
		return (move_to_path("HOME", curr_path, env_list));
	if (ft_strcmp(argv[1], "-") == 0)
		return (move_to_path("OLDPWD", curr_path, env_list));
	if (ft_strcmp(argv[1], ".") == 0)
		return (EXIT_SUCCESS);
	target_path = expand_relative_path(argv[1], curr_path);
	printf("%s\n", target_path);
	free(curr_path);
	// if (is_valid_path(argv[1]) == FALSE)
	// 	return (print_error(NOT_EXISTED, target_path));
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


*/

/*

	1. 인자가 유효한지 검사한다.
		- argv[1]이 존재하는 경로인가?
		- 상대 경로라면 . ./ .. ../ 로 시작하는가?
	
	2. getcwd 로 현재 위치를 받아온다.
		- . == getcwd 와 동일
		- .. == getcwd 에서 맨 마지막 경로만 제거

*/