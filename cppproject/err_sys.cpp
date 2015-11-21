#include "err_sys.h"
#include "stdlib.h"
void sub(int a, int b)
{
}

void err_sys(const char*msg)
{
	printf("%s",msg);
	exit(1);
}
