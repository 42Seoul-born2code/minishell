#include "minishell.h"

static t_bool	is_whitespace(char c)
{
	if (c == ' ' || c == '\n' || c == '\r' || c == '\v'
		|| c == '\f' || c == '\t')
		return (TRUE);
	return (FALSE);
}
// 2. >>> or <<<인지 확인
// 1. >> or << 인지 확인
// ft_strncmp는 0이여야 같은거임
// ft_strncmp에서 int개수 예외처리 해줘야함
t_bool	is_metacharacter(char *str)
{
	if (ft_strncmp(str, "<<<", 3) == 0 || \
	ft_strncmp(str, ">>>", 3) == 0)
	{
		printf("%s", "ERROR");
		return TRUE;
	}

	if (ft_strncmp(str, "<<", 2) == 0 || \
	ft_strncmp(str, ">>", 2) == 0)
	{
		printf("%s", "heredoc");
		return TRUE;
	}	

	if (*str== '|' || *str == '<' || *str == '>' || \
		*str == '\\')
	{
		return (TRUE);
	}
	return (FALSE);
}

enum e_meta get_meta_type(char c)
{
	if (is_whitespace(c) == TRUE)
		return (WHITESPACE);
	if (c == '|')
		return (PIPE);
	if (c == '<')
		return (REDIR_LEFT);
	// if (c == '<<')
		// return (REDIR_HEREDOC);
	if (c == '>')
		return (REDIR_RIGHT);
	// if (c == '>>')
		// return (REDIR_APPEND);
	if (c == '\\')
		return (BACKSLASH);
	if (c == '|')
		return (PIPE);
	else
		return (WORD);
}

void	tokenize_line(char *line, t_token *token_list)
{
	int				i;
	int				start;
	char			*word;
	t_token_node	*token_node;

	i = 0;
	word = NULL;
	(void)token_list;
	while (line[i] != '\0')
	{
		token_node = malloc(sizeof(t_token_node));
		if (is_whitespace(line[i]) || is_metacharacter(&line[i]))
		{
			token_node->type = get_meta_type(line[i]);
		}
		else
		{
			start = i;
			while (line[i] != '\0')
			{
				if (is_whitespace(line[i]) || is_metacharacter(&line[i]))
					break ;
				i += 1;
			}
			word = malloc(sizeof(char) * (i - start));
			ft_memcpy(word, &line[start], i - start);
			printf("word: %s\n", word);
		}
		i += 1;
	}
}

void	execute_minishell(t_env_list env)
{
	char	*line;
	char	*prompt;
	t_token	token_list;

	(void)env;
	prompt = "./minishell$ ";
	while (TRUE)
	{
		line = readline(prompt);
		if (!line)
			break ;
		tokenize_line(line, &token_list);
		add_history(line);
		free(line);
	}
}

/*

	echo "hello 'world'"
	echo "hello \"world\""
	cat < main.c
	ls > output
	ls -l | grep main
	ls -l | grep main >> output
	<< HELLO cat | wc -l >> outfile

*/