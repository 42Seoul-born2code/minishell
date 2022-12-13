#ifndef EXPANSION_H
# define EXPANSION_H

# include "tokenize.h"

void	expansion(t_token *token_list);

typedef struct s_word_list{
	t_list	*head_node;
}	t_word_list;

#endif