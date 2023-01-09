/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:18:57 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/08 20:20:29 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

typedef struct s_word_info{
	char		*word;
	t_meta		type;
	t_env_list	*env_list;
}	t_word_info;

// quote_removal.c
void		quote_removal(t_token *token_list);

// expansion_utils.c
t_bool		is_valid_variable_rule(char c);
char		*copy_str_length(int start, const char *src, int word_length);
void		save_before_env_variable(char *word, int *idx, \
					t_word_list *word_list, t_quote type);

// expansion.c
void		expansion(t_token *token_list, t_env_list *env_list);

// remove_whitespace.c
char		*remove_whitespace(char *str);

// save_expand_env_variable.c
void		save_expand_env_variable(t_word_info word_info, int *idx, \
					t_word_list *word_list, t_quote quote_type);

// save_word_and_expanded_variable.c
t_word_list	*save_word_and_expanded_variable(t_token_node *curr_token, \
											t_env_list *env_list);

#endif