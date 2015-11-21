#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdlib.h>
#include<fcntl.h>
int main()
{
	int in = open("hello.txt", O_RDONLY);
	int out = open("hello2.txt", O_RDWR);
	char buf[100];
	read(in, buf, 80);
	write(out, buf,80);
}
