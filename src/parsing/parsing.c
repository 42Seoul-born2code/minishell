/*

echo 'hello "world"' | cat > a

token_list

[echo]  ['hello "world"']   [|]      [cat]     [>]      [a]
word       word             pipe     word     rd     word

cmd         arg        			pipe    cmd   redirect   arg

<
>
<<
>>
|

if (node->next->type == PIPE)
if (node->next->type == REDIRECTION_LEFT)
if (node->next->type == REDIRECTION_RIGHT)
if (node->next->type == REDIRECTION_HEREDOC)
if (node->next->type == REDIRECTION_APPEND)


*/

#include "parsing.h"

void	parsing(t_token *token_list)
{
	t_list			*curr_node;
	t_token_node	*curr_content;

	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		curr_content = (t_token_node *)curr_node->content;
		printf("type: %d, word: %s\n", curr_content->type, curr_content->word);
		curr_node = curr_node->next;
	}
}
