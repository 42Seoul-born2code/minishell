#include "expansion.h"

/*

	echo '"hello"'
	-> "hello"
	
	echo "hello '$USER'"
	-> hello 'joonhan'

	echo hello'world'$USER
	-> helloworldjoonhan
	
	echo hello"'world'"joonpark
	-> hello'world'joonpark

	echo hello$USER
	-> hellojoonhan

	echo hello'"',dear:'"'$USER
	-> hello",dear:"joonhan

	'b''a'"s""h"
	-> bash

*/

/*

	환경변수 치환 -> word spliting (공백 제거) -> quote removal

	Note that if no expansion occurs, no splitting is performed.

	After the preceding expansions, all unquoted occurrences of the characters ‘\’, ‘'’, and ‘"’ (that did not result from one of the above expansions) are removed.

	export a="a       'a'"
	echo $a
	-> echo a         'a'
	-> a 'a'
	
	echo a 'a'
	-> a a

*/
void	quote_removal(t_token *token_list)
{
	int				idx;
	int				start_idx;
	int				word_length;
	t_list			*curr_node;
	t_token_node	*curr_token;
	char			*buffer;
	char			*prev_word;
	char			*result_word;
	char			start_quote;

	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		idx = 0;
		prev_word = NULL;
		result_word = NULL;
		buffer = NULL;
		curr_token = curr_node->content;
		while (curr_token->word[idx] != '\0')
		{
			if (is_quote(curr_token->word[idx]) == TRUE)
			{
				start_quote = curr_token->word[idx];
				idx += 1;
				start_idx = idx;
				// 1-1. 끝나는 따옴표 위치 파악
				while (curr_token->word[idx] != '\0' && curr_token->word[idx] != start_quote)
				{
					idx += 1;
				}
				// 1-2. 따옴표 제외한 문자열 복사
				if (start_idx != idx)
				{
					word_length = idx - start_idx;
					prev_word = malloc(sizeof(char) * word_length + 1);
					ft_memcpy(prev_word, &curr_token->word[start_idx], word_length);
					idx += 1;
				}
			}

			// 2. 따옴표가 아닌 경우
			else
			{
				start_idx = idx;
				while (curr_token->word[idx] != '\0')
				{
					if (is_quote(curr_token->word[idx]) == TRUE)
						break ;
					idx += 1;
				}
				if (start_idx != idx)
				{
					word_length = idx - start_idx;
					prev_word = malloc(sizeof(char) * word_length + 1);
					ft_memcpy(prev_word, &curr_token->word[start_idx], word_length);
				}
			}

			// 1-3. 따옴표 이전 문자열과 strjoin
			if (result_word != NULL)
			{
				buffer = ft_strdup(result_word);
				free(result_word);
			}
			result_word = ft_strjoin(buffer, prev_word);
			free(prev_word);
			prev_word = NULL;
		}
		printf("result_word: %s\n", result_word);
		free(curr_token->word);
		curr_token->word = result_word;
		curr_node = curr_node->next;
	}
}