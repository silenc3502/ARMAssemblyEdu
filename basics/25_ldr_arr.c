#include <stdio.h>

unsigned int arr[6] = {1, 2, 3, 4, 5};

int main(void)
{
	register unsigned int r0 asm("r0") = 0;
	register unsigned int *r1 asm("r1") = 0;
	register unsigned int r2 asm("r2") = 0;

	r1 = arr;

	asm volatile("mov r2, #0x8");
	asm volatile("ldr r0, [r1, r2]");

	printf("r0 = %u\n", r0);

	return 0;
}
