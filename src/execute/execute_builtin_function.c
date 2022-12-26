#include "execute.h"
#include "builtin.h"
void	execute_builtin_function(char *cmd, char **argv, char **envp, t_env_list *env_list)
{
	(void)(argv);
	(void)(env_list);
	(void)(envp);
	if (ft_strcmp(cmd, "echo") == 0)
	{
		ft_echo(argv);
		return ;
	}
	else if (ft_strcmp(cmd, "cd") == 0)
	{
		// ft_cd(argv);
		return ;
	}
	else if (ft_strcmp(cmd, "pwd") == 0)
	{
		ft_pwd(argv);
		return ;
	}
	else if (ft_strcmp(cmd, "export") == 0)
	{
		// ft_export(argv, env_list);
		return ;
	}
	else if (ft_strcmp(cmd, "unset") == 0)
	{
		// ft_unset(argv);
		return ;
	}
	else if (ft_strcmp(cmd, "env") == 0)
	{
		// ft_env(argv);
		return ;
	}
	else if (ft_strcmp(cmd, "exit") == 0)
	{
		// ft_exit(argv);
		return ;
	}
}
