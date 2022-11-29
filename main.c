#include "minishell.h"
#include <stdio.h>


void	handler(int signum)
{
	pid_t	pid;
	int		status;

	pid = waitpid(-1, &status, WNOHANG);
	if (signum == SIGINT)
	{
		if (pid == -1)
		{

		}
	}
}


// set_signal(void)
// {
// 	signal(SIGINT, hadler);
// 	signal(SIGQUIT, handler);
// }

char	*ft_strdup(const char *s1)
{
	char	*p;
	int		i;
	int		len;

	if (s1 == 0)
		return (0);
	len = 0;
	while (s1[len])
		len++;
	p = (char *)malloc(sizeof(char) * (len + 1));
	if (p == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		p[i] = s1[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}


char			**get_envp(char **envp)
{
	char	**g_envp;
	int		i;

	i = 0;
	while (envp[i] != NULL)
	{
		i++;
		printf("%s\n",envp[i]);
	}
	if (!(g_envp = malloc(sizeof(char *) * (i + 1))))
		return (NULL);
	i = -1;
	while (envp[++i])
		g_envp[i] = ft_strdup(envp[i]);
	g_envp[i] = NULL;
	return (g_envp);
}

// TODO 연결리스트 구조체 생성하기 [v]
// TODO 환경변수를 연결리스트로 저장하기 []
// TODO 환경변수 연결리스트 끝에 추가하기 (libft 에서 만든 함수)
// TODO 연결리스트를 더블 포인터 형태로 만들기

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	*prompt;
	char	**g_envp;
	t_envp	local_envp;

	g_envp = NULL;
	if (argc != 1)
		return (EXIT_FAILURE);
	(void)argv;
	g_envp = get_envp(envp);
	prompt = "./minishell$ ";
	printf("HELLO MINISHELL!\n");
	while (TRUE)
	{
		input = readline(prompt);
		if (!input)
			break;
		add_history(input);
		free(input);
	}
	return (0);
}