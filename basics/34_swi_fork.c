#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int i;

	register unsigned int r0 asm("r0") = 0;
	register unsigned int r7 asm("r7") = 0;

	asm volatile("mov r7, #2");
	asm volatile("swi #0" : "=r" (r0) : "r" (r7) : "memory");

	if(r0 > 0)
		printf("r0 = %u, parent\n", r0);
	else if(!r0)
		printf("r0 = %u, child\n", r0);
	else
	{
		perror("fork() ");
		exit(-1);
	}

	return 0;
}
