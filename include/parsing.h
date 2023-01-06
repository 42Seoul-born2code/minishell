/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:18:46 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/06 16:19:01 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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