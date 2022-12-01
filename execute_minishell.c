#include "minishell.h"

static t_bool	is_whitespace(char c)
{
	if (c == ' ' || c == '\n' || c == '\r' || c == '\v'
		|| c == '\f' || c == '\t')
		return (TRUE);
	return (FALSE);
}

t_bool	is_metacharacter(char *str)
{
	// TODO 3개짜리는 별도로 어떻게 처리할지?
	// if (ft_strncmp(str, "<<<", 3) == 0 || \
	// ft_strncmp(str, ">>>", 3) == 0)
	// {
	// 	printf("%s", "ERROR");
	// 	return (FALSE);
	// }
	if (ft_strncmp(str, "<<", 2) == 0 || \
	ft_strncmp(str, ">>", 2) == 0)
	{
		return (TRUE);
	}	
	if (*str== '|' || *str == '<' || *str == '>' || \
		*str == '\\')
	{
		return (TRUE);
	}
	return (FALSE);
}

e_meta get_meta_type(char *str)
{
	if (is_whitespace(*str) == TRUE)
		return (WHITESPACE);
	if (*str == '|')
		return (PIPE);
	if (ft_strncmp(str, "<<", 2) == 0)
		return (REDIR_HEREDOC);
	if (*str == '<')
		return (REDIR_LEFT);
	if (ft_strncmp(str, ">>", 2) == 0)
		return (REDIR_APPEND);
	if (*str == '>')
		return (REDIR_RIGHT);
	if (*str == '\\')
		return (BACKSLASH);
	if (*str == '|')
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
	while (line[i] != '\0')
	{
		token_node = malloc(sizeof(t_token_node));
		if (is_whitespace(line[i]) || is_metacharacter(&line[i]))
		{
			token_node->type = get_meta_type(&line[i]);
			if (token_node->type == REDIR_HEREDOC || token_node->type == REDIR_APPEND)
				i += 1;
			token_node->word = NULL;
			i += 1;
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
			word = malloc(sizeof(char) * (i - start + 1));
			ft_memcpy(word, &line[start], i - start);
			token_node->word = word;
			token_node->type = WORD;
		}
		ft_lstadd_back(&token_list->head_node, ft_lstnew(token_node));
	}
}

void	execute_minishell(t_env_list env)
{
	char	*line;
	char	*prompt;
	t_token	*token_list;

	(void)env;
	prompt = "./minishell$ ";
	token_list = malloc(sizeof(t_token));
	token_list->head_node = NULL;
	while (TRUE)
	{
		line = readline(prompt);
		if (!line)
			break ;
		tokenize_line(line, token_list);
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