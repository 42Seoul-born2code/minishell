#include "tokenize.h"

/*		AWESOME_TEST_CASE		*/
/*

echo "hello 'world' > outfile"
echo 'hello "world"' | cat > a
echo aaa"kk"kk haha > ee
  echo 'hello "world"' |>
ls | cat > outfile
'ba''sh'
'ba'"sh"
'ba'sh
'b''a'"s""h" 

*/
// 1. 공백(whitespace)이면 건너뛴다.
// 2. metacharacter 로 구분되지 않은 따옴표이면, 
//		metacharacter 를 만날 때까지, 그리고 닫는 따옴표를 만날 때까지 인덱스를 증가시킨다.

void	tokenize_line(char *line, t_token *token_list)
{
	int				i;
	int				start;
	t_bool			is_quote_closed;
	char			*word;
	char			quote_type;
	t_token_node	*token_node;
	int				word_length;

	i = 0;
	word = NULL;
	(void)token_list;
	while (line[i] != '\0')
	{
		// CASE1. whitespace 인 경우
		if (is_whitespace(line[i]) == TRUE)
		{
			i += 1;
			continue ;
		}
		// CASE2. operator 이면 무조건 저장
		token_node = malloc(sizeof(t_token_node));
		if (is_operator(&line[i]) == TRUE)
		{
			token_node->type = get_meta_type(&line[i]);
			if (token_node->type == REDIR_HEREDOC || token_node->type == REDIR_APPEND)
			{
				word_length = 2;
				i += 2;
			}
			else
			{
				word_length = 1;
				i += 1;
			}
			word = malloc(sizeof(char) * (word_length + 1));
			ft_memcpy(word, &line[i - (word_length)], word_length);
			token_node->word = word;
			ft_lstadd_back(&token_list->head_node, ft_lstnew(token_node));
		}
		
		// CASE3. 따옴표를 만났을 때
		else if (is_quote(line[i]) == TRUE)
		{
			start = i;
			is_quote_closed = FALSE;
			// 따옴표로 묶인 문자들을 연속으로 만났을 때 하나의 word 로 처리하기 위한 반복문
			while (line[i] != '\0')
			{
				// 'b'a's'"h"\0
				if (is_quote(line[i]) == FALSE && (is_whitespace(line[i]) == TRUE || is_operator(&line[i]) == TRUE))
					break ;
				if (is_quote(line[i]) == TRUE)
				{
					quote_type = line[i];
					i += 1;
					while (line[i] != '\0' && quote_type != line[i])
					{
						i += 1;
					}
					if (line[i] == '\0')
					{
						is_quote_closed = FALSE;
						break ;
					}
					is_quote_closed = TRUE;
					if (is_whitespace(line[i]) && is_operator(&line[i]))
						break ;
				}
				i += 1;
			}
			// 따옴표가 닫히지 않은 경우를 확인하기 위한 조건
			if (is_quote_closed == FALSE)
			{
				printf("앗챠챠! 따옴표가 안닫혔데스네!\n");
				// TODO 오류 메세지 출력하도록 구현(Syntax Error)
			}
			else
			{
				word_length = (i - start) + 1;
				if (is_whitespace(line[i]) || is_operator(&line[i]))
				{
					i -= 1;
					word_length -= 1;
				}
				word = malloc(sizeof(char) * (word_length + 1));
				ft_memcpy(word, &line[start], word_length);
				token_node->word = word;
				token_node->type = WORD;
				ft_lstadd_back(&token_list->head_node, ft_lstnew(token_node));
				i += 1;
			}
		}

		// CASE4. word 를 만났을 때
		// ec'ho' 'hello "world"'
		// [ec'ho', 'hello "world"']
		else
		{
			// word인 경우
			start = i;
			while (line[i] != '\0' && is_whitespace(line[i]) == FALSE && is_operator(&line[i]) == FALSE)
			{
				i += 1;
			}
			// i : 6, start : 0
			word_length = (i - start);
			word = malloc(sizeof(char) * (word_length + 1));
			ft_memcpy(word, &line[start], word_length);
			token_node->word = word;
			token_node->type = WORD;
			ft_lstadd_back(&token_list->head_node, ft_lstnew(token_node));
		}
	}
}