#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int fd = 1; // 표준 출력
	char buf[] = "Hello, World!\n";

	write(fd, buf, sizeof(buf) -1);
	close(fd);

	return 0;
}