/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:18:06 by jeongkpa          #+#    #+#             */
/*   Updated: 2023/01/06 16:18:44 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expansion.h"
#include "utils.h"

// 환경변수를 확장하기 위해 연결 리스트로 저장했던 문자열들을 하나로 합치는 함수
static void	merge_replaced_word(t_word_list *word_list, char **replaced_word)
{	
	char	*previous_word;
	t_list	*curr_word;
	t_list	*next_word;

	curr_word = word_list->head_node;
	while (curr_word != NULL)
	{
		next_word = curr_word->next;
		previous_word = ft_strdup(*replaced_word);
		free(*replaced_word);
		*replaced_word = ft_strjoin(previous_word, (char *)curr_word->content);
		free(previous_word);
		free(curr_word->content);
		free(curr_word);
		curr_word = next_word;
	}
}

/*
	환경 변수를 확장해서 기존의 토큰을 교체하는 함수

	1. 작은 따옴표 안에 있는 문자열
	2. 큰 따옴표 안에 있는 문자열
	3. 따옴표에 둘러 쌓이지 않은 문자열
*/
void	expansion(t_token *token_list, t_env_list *env_list)
{
	char			*replaced_word;
	t_list			*curr_node;
	t_token_node	*curr_token;
	t_word_list		*word_list;

	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		replaced_word = NULL;
		curr_token = curr_node->content;
		word_list = save_word_and_expanded_variable(curr_token, env_list);
		merge_replaced_word(word_list, &replaced_word);
		if (replaced_word != NULL)
		{
			free(curr_token->word);
			curr_token->word = replaced_word;
		}
		free(word_list);
		curr_node = curr_node->next;
	}
}

// echo $USER$NOT$$"h"w''$
// echo '"$USER"'$USER