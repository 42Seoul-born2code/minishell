#include "minishell.h"
#include <stdio.h>


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

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	*prompt;
	char	**g_envp;

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