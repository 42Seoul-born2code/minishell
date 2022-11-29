#include "minishell.h"

// void	sig_handler(int signo)
// {
// 	pid_t	pid;
// 	int		status;

// 	pid = waitpid(-1, &status, WNOHANG);
// 	if (signo == SIGINT)
// 	{
// 		if (pid == -1)
// 		{
// 			ft_putstr_fd("\n", STDOUT_FILENO);
// 			printf("minishell$");
// 		}
// 		else
// 			ft_putchar_fd('\n', STDOUT_FILENO);
// 	}
// 	else if (signo == SIGQUIT)
// 	{
// 		if (pid == -1)
// 			ft_putstr_fd("", STDOUT_FILENO);
// 		else
// 			ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
// 	}
// }

// set_signal(void)
// {
// 	signal(SIGINT, sig_handler);
// 	signal(SIGQUIT, sig_handler);
// }

// TODO 연결 리스트를 다시 char ** 으로 만들 때 사용하기
// char	**get_envp(char **envp)
// {
// 	char	**g_envp;
// 	int		i;

// 	i = 0;
// 	while (envp[i] != NULL)
// 	{
// 		i++;
// 	}
// 	if (!(g_envp = malloc(sizeof(char *) * (i + 1))))
// 		return (NULL);
// 	i = -1;
// 	while (envp[++i])
// 		g_envp[i] = ft_strdup(envp[i]);
// 	g_envp[i] = NULL;
// 	return (g_envp);
// }

void	save_envp_in_list(char **envp, t_env_list *curr_env)
{
	int			i;
	char		**kv_pair;
	t_env_node	*env_node;

	i = 0;
	curr_env->head_node = NULL;
	while (envp[i] != NULL)
	{
		printf("%s\n", envp[i]);
		kv_pair = ft_split(envp[i], '=');
		env_node = malloc(sizeof(t_env_node));
		env_node->line = ft_strdup(envp[i]);
		env_node->key = kv_pair[0];
		env_node->value = kv_pair[1];
		ft_lstadd_back(&curr_env->head_node, ft_lstnew(env_node));
		i += 1;
	}
}

// TODO 연결리스트 구조체 생성하기 [v]
// TODO 환경변수를 연결리스트로 저장하기 [v]
// TODO 연결리스트를 더블 포인터 형태로 만들기 []

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	char		*prompt;
	t_env_list	curr_env;

	(void)envp;
	if (argc != 1)
		return (EXIT_FAILURE);
	prompt = "./minishell$ ";
	save_envp_in_list(envp, &curr_env);
	(void)argv;
	// while (TRUE)
	// {
	// 	input = readline(prompt);
	// 	if (!input)
	// 		break;
	// 	add_history(input);
	// 	free(input);
	// }
	// system("leaks minishell");
	return (0);
}