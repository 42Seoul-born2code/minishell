/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:18:44 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/06 16:18:53 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

# include "libft.h"
# include <stdio.h>

# define NOT_CLOSED_QUOTE	"Error: Quote is not closed. : "

typedef enum e_meta{
	WHITESPACE,
	WORD,
	PIPE,
	REDIR_LEFT,
	REDIR_HEREDOC,
	REDIR_RIGHT,
	REDIR_APPEND,
	COMMAND,
	ARGUMENT,
	LIMITER,
	FILE_NAME,
}	t_meta;

typedef struct s_token {
	t_list	*head_node;
}	t_token;

typedef struct s_token_node {
	t_meta	type;
	char	*word;
}	t_token_node;

// tokenize_line.c
int		tokenize_line(char *line, t_token *token_list);

// tokenize_utils.c
t_bool	is_operator(char *str);
t_bool	is_quote(char c);
t_meta	get_meta_type(char *str);
void	get_operator_type(t_token_node *node, char *line, \
								int *idx, int *length);

#endif