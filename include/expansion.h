#ifndef EXPANSION_H
# define EXPANSION_H

# include "tokenize.h"

void	expansion(t_token *token_list);
void	quote_removal(t_token *token_list);
t_bool 	is_valid_variable_rule(char c);

typedef enum {
	NOT_QUOTED,
	QUOTED
}	e_quote;

typedef struct s_word_list{
	t_list	*head_node;
}	t_word_list;

#endif