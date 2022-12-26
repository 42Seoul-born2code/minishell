#include "builtin.h"

/*

	CHESEO's advice

	1. builtin function 실행 후 exit status

	2. pwd : cd 를 하고나서 pwd 를 업데이트가 되어야 한다.

	3. cd 
		- cd 실패 시 어떻게 되어야 하는건지? (에러 메세지 출력)
		- chdir 상대 경로 또는 절대 경로로 주어야 함
		- 바로 반영이 안될 수 있다.

*/

int ft_echo(char **argv)
{
	(void)argv;
	return 0;
}