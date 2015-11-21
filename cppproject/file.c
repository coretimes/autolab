#include <stdio.h>
int main()
{
	FILE * pF = fopen("vi.h","wb");
	fprintf(pF, "hello");
}
