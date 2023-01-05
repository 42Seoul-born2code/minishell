#include "execute.h"
#include "libft.h"

void	execute_cmd(char *cmd_name, char **cmd_argv, t_env_list *env_list)
{
	char	*cmd_path;
	char	**envp;

	cmd_path = find_cmd_path(cmd_name, env_list);
	if (cmd_path == NULL)
	{
		print_error(COMMAND_NOT_FOUND, cmd_name);
		exit(ERROR_CODE_COMMAND_NOT_FOUND);
	}
	if (cmd_argv == NULL)
		exit(EXIT_SUCCESS);
	envp = get_envp_in_list(env_list);
	if (execve(cmd_path, cmd_argv, envp) == ERROR)
	{
		print_error(COMMAND_NOT_FOUND, cmd_path);
		exit(ERROR_CODE_COMMAND_NOT_FOUND);
	}
}
