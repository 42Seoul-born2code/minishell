#ifndef EXPANSION_H
# define EXPANSION_H

# include "tokenize.h"
# include "utils.h"

typedef enum e_quote{
	NOT_QUOTED,
	QUOTED,
	SINGLE_QUOTED,
	DOUBLE_QUOTED
}	t_quote;

typedef struct s_word_list{
	t_list	*head_node;
}	t_word_list;

void	expansion(t_token *token_list, t_env_list *env_list);
void	quote_removal(t_token *token_list);
t_bool	is_valid_variable_rule(char c);

// expansion.c
void	save_before_env_variable(char *word, int *idx, \
					t_word_list *word_list, t_quote type);
void	save_expand_env_variable(t_token_node *token, int *idx, \
					t_word_list *word_list, t_env_list *env_list, t_quote quote_type);

// save_word_and_expanded_variable.c
t_word_list	*save_word_and_expanded_variable(t_token_node *curr_token, t_env_list *env_list);

#endif