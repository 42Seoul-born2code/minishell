/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:19:44 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/11 14:50:53 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <dirent.h>

/*
	매개변수로 전달된 환경변수의 경로로 현재 위치를 변경
	- env_path: 이동할 환경변수
	- env_list: 환경변수 리스트
*/
static int	move_to_env_path(char *env_path, t_env_list *env_list)
{
	char	*curr_path;
	char	*target_path;

	if (is_env_existed(env_list, env_path) == FALSE)
		return (print_error(VARIABLE_IS_UNSET, env_path));
	curr_path = ft_strdup(get_env_value(env_list, "PWD"));
	target_path = get_env_value(env_list, env_path);
	if (chdir(target_path) == ERROR)
	{
		print_error(NO_SUCH_FILE_OR_DIR, target_path);
		free(curr_path);
		return (EXIT_FAILURE);
	}
	replace_env_value(env_list, "PWD", target_path);
	replace_env_value(env_list, "OLDPWD", curr_path);
	free(curr_path);
	return (EXIT_SUCCESS);
}

/*
	실제로 현재 디렉토리를 변경하는 함수
	1. 폴더가 존재하는지 먼저 확인: stat 이나 lstat 으로 먼저 확인
	2. 접근 가능한지 확인: opendir 로 확인
	3. 상대 경로 -> 절대 경로 순서대로 확인: 순차적으로 확인
*/

/*
	1. 상대 경로 존재 O 접근 O
	2. 상대 경로 존재 O 접근 X
	3. 상대 경로 존재 X 접근 O/X
	4. 절대 경로 존재 O 접근 O
	5. 절대 경로 존재 O 접근 X
	6. 절대 경로 존재 X 접근 O/X
*/
int	change_directories(char *argv, char **paths, t_env_list *env_list)
{
	int		idx;
	int		result;
	char	*old_pwd;

	idx = -1;
	old_pwd = getcwd(NULL, BUFSIZ);
	result = try_to_move_directories(idx, argv, paths, env_list);
	if (result != EXIT_ERROR)
		replace_env_value(env_list, "OLDPWD", old_pwd);
	free(old_pwd);
	free_all(paths);
	return (result);
}

int	ft_cd(char **argv, t_env_list *env_list)
{
	char	*old_pwd;
	char	**paths;

	if (argv[1] == NULL)
	{
		if (is_env_existed(env_list, "HOME") == TRUE)
			return (move_to_env_path("HOME", env_list));
		return (print_error(HOME_IS_UNSET, NULL));
	}
	if (ft_strcmp(argv[1], "-") == 0 || ft_strcmp(argv[1], "--") == 0)
		return (move_to_env_path("OLDPWD", env_list));
	if (ft_strcmp(argv[1], "/") == 0)
	{
		old_pwd = getcwd(NULL, BUFSIZ);
		replace_env_value(env_list, "PWD", "/");
		replace_env_value(env_list, "OLDPWD", old_pwd);
		free(old_pwd);
		chdir("/");
		return (EXIT_SUCCESS);
	}
	paths = ft_split(argv[1], '/');
	return (change_directories(argv[1], paths, env_list));
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