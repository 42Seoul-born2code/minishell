#include "tokenize.h"

t_bool	is_whitespace(char c)
{
	if (c == ' ' || c == '\n' || c == '\r' || c == '\v'
		|| c == '\f' || c == '\t')
		return (TRUE);
	return (FALSE);
}

t_bool	is_operator(char *str)
{
	if (ft_strncmp(str, "<<", 2) == 0 || \
	ft_strncmp(str, ">>", 2) == 0)
	{
		return (TRUE);
	}	
	if (*str == '|' || *str == '<' || *str == '>')
	{
		return (TRUE);
	}
	return (FALSE);
}

t_bool	is_quote(char c)
{
	if (c == '\'' || c == '\"')
	{
		return (TRUE);
	}
	return (FALSE);
}

t_meta	get_meta_type(char *str)
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
	if (*str == '|')
		return (PIPE);
	else
		return (WORD);
}
