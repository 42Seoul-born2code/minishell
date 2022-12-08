#ifndef PARSING_H
# define PARSING_H

# include "libft.h"
# include "tokenize.h"

typedef struct s_parse_info {
	t_bool	is_command_found;
	t_bool	is_redirection_found;
	t_bool	is_heredoc_found;
}	t_parse_info;

void	parsing(t_token *token_list);

#endif