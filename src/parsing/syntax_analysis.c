#include "parsing.h"

// void	syntax_analysis(t_token *token_list)
// {

// }
/*

	1. 따옴표가 제대로 닫혔는가?
		- [[echo], ['hello "world"]] | cat

	2. operator 바로 뒤에 operator 인가?
		- | <
		- | >
		- | <<
		- | >>
		- < |
		- < <
		- < <<
		- ...

	3. 각 오퍼레이터 별로 어순이 맞는가?
		- Redirection left (<)
			- [COMMAND < FILE_NAME]
			- [< FILE_NAME COMMAND]
		- Pipe (|)
			- | [COMMAND]


*/