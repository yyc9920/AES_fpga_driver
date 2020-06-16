#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main()
{
	char tmp[10];
	int a;
	a = 5000;
	sprintf(tmp, "%d", a);
	printf("%s\n", tmp);
	a = -5000;
	sprintf(tmp, "%d", a);
	printf("%s\n", tmp);
}
