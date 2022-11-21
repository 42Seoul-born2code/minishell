#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#define TRUE 1
/*
char *ttyname(int desc);
ttyname()는 열린파일 지정자 desc에 대한 터미널 이름을 얻어온다. 
만약 터미널 이름을 가져오는데 실패 했다면 NULL을 되돌려 준다.

*/
int	main(void)
{
	if (isatty(STDERR_FILENO) == TRUE)
	{
		printf("Your tty name is : %s\n", ttyname(STDERR_FILENO));
	}
}
