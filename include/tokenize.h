#ifndef TOKENIZE_H
# define TOKENIZE_H

# include "libft.h"
# include <stdio.h>

typedef enum {
	WHITESPACE,
	WORD,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	DOLLAR,
	EQUAL,
	PIPE,
	REDIR_LEFT,
	REDIR_HEREDOC,
	REDIR_RIGHT,
	REDIR_APPEND
}	e_meta;

typedef struct s_token {
	t_list	*head_node;
}	t_token;

typedef struct s_token_node {
	e_meta	type;
	char	*word;
}	t_token_node;

void	tokenize_line(char *line, t_token *token_list);
t_bool	is_whitespace(char c);
t_bool	is_operator(char *str);
t_bool	is_quote(char c);
e_meta 	get_meta_type(char *str);

#endif