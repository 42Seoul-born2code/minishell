#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#define TRUE 1

/*
int ioctl(int d, int request, ...);
첫번째 인자는 open한 디바이스 드라이버의 fd 값이다
두번째 인자는 디바이스에게 전달할 명령이다. 이 명령에 따라서 디바이스를 컨트롤 할 수 있다.
*/

int	main(void)
{
	struct winsize	size;

	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &size) == -1)
		return (1);
	printf("Terminal Row Size:\t%d\n",size.ws_row);
	printf("Terminal Col Size:\t%d\n",size.ws_col);
	return (0);
}