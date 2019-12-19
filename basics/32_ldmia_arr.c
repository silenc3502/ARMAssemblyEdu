#include <stdio.h>

unsigned int arr[6];

void print_for(void)
{
	int i;

	for(i = 0; i < 6; i++)
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
	register unsigned int r5 asm("r5") = 0;
	register unsigned int r6 asm("r6") = 0;
	register unsigned int *r9 asm("r9") = 0;

	r9 = arr;

	asm volatile("mov r1, #3");
	asm volatile("mov r2, r1, lsl #2");
	asm volatile("mov r4, #2");
	asm volatile("add r3, r1, r2, lsl r4");
	asm volatile("stmia r9!, {r1, r2, r3}");
	asm volatile("str r4, [r9]");
	asm volatile("mov r5, #128");
	asm volatile("mov r6, r5, lsr #3");
	asm volatile("stmia r9, {r4, r5, r6}");
	asm volatile("sub r9, r9, #12");
	asm volatile("ldmia r9, {r4, r5, r6}");

	print_for();

	printf("r4 = 0x%x, r5 = 0x%x, r6 = 0x%x\n", r4, r5, r6);

	return 0;
}
