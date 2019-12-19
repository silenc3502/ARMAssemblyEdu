#include <stdio.h>

unsigned int arr[6];

void print_for(void)
{
	int i;

	for(i = 0; i < 5; i++)
		printf("arr[%d] = %d\n", i, arr[i]);
}

int main(void)
{
	int i;

	register unsigned int r0 asm("r0") = 0;
	register unsigned int r1 asm("r1") = 0;
	register unsigned int r2 asm("r2") = 0;
	register unsigned int r3 asm("r3") = 0;
	register unsigned int r4 asm("r4") = 0;
	register unsigned int *r5 asm("r5") = 0;

	r5 = arr;

	asm volatile("mov r1, #3");
	asm volatile("mov r2, r1, lsl #2");
	asm volatile("mov r4, #2");
	asm volatile("add r3, r1, r2, lsl r4");
	asm volatile("stmia r5, {r1, r2, r3}");

	print_for();

	return 0;
}
