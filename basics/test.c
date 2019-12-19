#include <stdio.h>

static int i = 3;
static char str[] = "Hello World, %d\n";
static char str2[] = "Hello World, %s\n";
static char ch[] = {'A', 'B', 0};
static int ary[] = {0, 1, 2, 3, 4};

int main(void)
{
	//printf(str, i);
	//printf(str2, ch);
	//float farr[] = {0.1, 0.2, 0.3, 0.4, 0.5};
	float a = 0.3;
	float res, b = 0.7;

	//res = farr[2] + b;
	res = a + b;
	printf("float = %f\n", res);

	return 0;
}
