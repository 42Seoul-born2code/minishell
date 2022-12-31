#include "execute.h"
#include "builtin.h"

int	execute_builtin_function(char *cmd, char **argv, t_env_list *env_list)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (ft_echo(argv));
	else if (ft_strcmp(cmd, "cd") == 0)
		return (ft_cd(argv, env_list));
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (ft_pwd(argv, env_list));
	else if (ft_strcmp(cmd, "export") == 0)
		return (ft_export(argv, env_list));
	else if (ft_strcmp(cmd, "unset") == 0)
		return (ft_unset(argv, env_list));
	else if (ft_strcmp(cmd, "env") == 0)
		return (ft_env(argv, env_list));
	else if (ft_strcmp(cmd, "exit") == 0)
		ft_exit(argv, env_list);
	return (EXIT_SUCCESS);
}
