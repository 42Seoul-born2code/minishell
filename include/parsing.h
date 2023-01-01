#ifndef PARSING_H
# define PARSING_H

# include "libft.h"
# include "tokenize.h"

typedef struct s_parse_info {
	t_bool	is_command_found;
	t_bool	is_redirection_found;
	t_bool	is_heredoc_found;
}	t_parse_info;

typedef struct s_unit {
	t_list	*head_node;
}	t_unit;

# define SYNTAX_OK	2

void	parsing(t_token *token_list);
int		syntax_analysis(t_token *token_list);

#endif