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
