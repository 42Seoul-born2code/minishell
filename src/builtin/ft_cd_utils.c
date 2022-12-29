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

/*
	현재 디렉토리의 상위 디렉토리 경로를 가져오는 함수
*/
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

/*
	슬래시(/)로 구분된 경로로 만드는 함수
*/
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
