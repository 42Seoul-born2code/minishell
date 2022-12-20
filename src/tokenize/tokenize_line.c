#include "tokenize.h"

static void	save_token(t_token *token_list, t_token_node *node, char *line, int length)
{
	char	*word;

	word = malloc(sizeof(char) * (length + 1));
	ft_memcpy(word, line, length);
	node->word = word;
	ft_lstadd_back(&token_list->head_node, ft_lstnew(node));	
}

static void	get_operator_type(t_token_node *node, char *line, int *i, int *length)
{
	node->type = get_meta_type(&line[*i]);
	if (node->type == REDIR_HEREDOC || node->type == REDIR_APPEND)
	{
		*length = 2;
		*i += 2;
	}
	else
	{
		*length = 1;
		*i += 1;
	}
}

// 따옴표로 묶인 문자들을 연속으로 만났을 때 하나의 word 로 처리하기 위한 반복문
static t_bool	is_quote_closed(char *line, int *i)
{
	char	quote_type;
	t_bool	result;

	result = FALSE;
	if (is_quote(line[*i]) == FALSE)
	{
		*i += 1;
		return (TRUE);
	}
	quote_type = line[*i];
	*i += 1;
	while (line[*i] != '\0')
	{
		if (quote_type == line[*i])
		{
			result = TRUE;
			*i += 1;
			break ;
		}
		*i += 1;
	}
	return (result);
}

// ls" "'
static int	get_word_length(t_token_node *node, char *line, int *i, int start)
{
	while (line[*i] != '\0')
	{
		if (is_quote(line[*i]) == TRUE)
		{
			if (is_quote_closed(line, i) == FALSE)
				return (ERROR);
			else
				continue ;
		}
		if (is_whitespace(line[*i]) == TRUE || is_operator(&line[*i]) == TRUE)
		{
			break ;
		}
		*i += 1;
	}
	node->type = WORD;
	return (*i - start);
}

void	tokenize_line(char *line, t_token *token_list)
{
	int				i;
	int				start;
	int				word_length;
	t_token_node	*token_node;

	i = 0;
	while (line[i] != '\0')
	{
		// CASE1. whitespace 인 경우
		if (is_whitespace(line[i]) == TRUE)
		{
			i += 1;
			continue ;
		}
		// CASE2. operator 이면 무조건 저장
		start = i;
		token_node = malloc(sizeof(t_token_node));
		if (is_operator(&line[i]) == TRUE)
		{
			get_operator_type(token_node, line, &i, &word_length);
		}
		
		// CASE3. 따옴표를 만났을 때
		// 1. "hello$NAME".hi -> 따옴표가 닫히지 않았다고 문법 오류를 뱉어냄 [ 해결 ] 
		// 2. "hello$NAME".hi -> "hellojoonpark.hi" expansion 문제 해결 필요 [ 해결 ]
		// 3. expansion에서 큰따옴표 처리를 잘 못하고 있음 버그 발견 예제 : 'b''a'"s""h" [ 해결 ]
		// 4. "$NAME$" 입력 시 -> "joonparkjoonpark" 으로 출력되는 문제 발견 [ 해결 ]
		// 5. echo ls" " 입력 시 공백이 들어오면 토큰을 잘라서 인식함 []
		else if (is_quote(line[i]) == TRUE)
		{
			while (line[i] != '\0')
			{
				if (is_quote_closed(line, &i) == FALSE)
				{
					printf("앗챠챠! 따옴표가 안닫혔데스네!\n");
					break ;
				}
				if (is_operator(&line[i]) == TRUE || is_whitespace(line[i]) == TRUE)
				{
					break ;
				}
			}
			word_length = i - start;
			token_node->type = WORD;
		}

		// CASE4. word 를 만났을 때
		else
		{
			word_length = get_word_length(token_node, line, &i, start);
			if (word_length == ERROR)
			{
				printf("앗챠챠! 따옴표가 안닫혔데스네!\n");
				break ;
			}
		}
		save_token(token_list, token_node, &line[start], word_length);
	}
}