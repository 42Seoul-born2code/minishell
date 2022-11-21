#include <stdlib.h>
#include <stdio.h>

int	main(void)
{
	// 환경변수는 대소문자를 구분한다.
	printf("%s\n", getenv("joonhan"));
	printf("%s\n", getenv("JOONHAN"));
	// cat $JOONHAN 
	// 정의되지 않은 환경변수를 실행시키면 무한 대기에 걸린다
	// cd $JOONHAN
	// 정의되지 않은 환경변수를 넣으면 \0 문자로 인식하는 것 같다. (루트 디렉토리로 이동)
}
