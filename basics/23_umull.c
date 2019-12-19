#include <stdio.h>

#define WORD_SIZE		4

void print64(unsigned int r1, unsigned int r0)
{
	int i;
	unsigned char t = 0xf;

	if(!r1)
		printf("r1r0 = 0x%x\n", r1);
	else if(r0 < 0x10000000)
	{
		printf("r1r0 = 0x%x", r1);

		for(i = 7; i >= 0; i--)
		{
			if((r0 >> (i * WORD_SIZE)) & t)
				printf("%x", (r0 >> (i * WORD_SIZE)) & t);
			else
				printf("%x", 0);
		}

		printf("\n");
	}
}

int main(void)
{
	register unsigned int r0 asm("r0") = 0;
	register unsigned int r1 asm("r1") = 0;
	register unsigned int r2 asm("r2") = 0;
	register unsigned int r3 asm("r3") = 0;
	register unsigned int r4 asm("r4") = 0;
	register unsigned int r5 asm("r5") = 0;

	asm volatile("mov r2, #0x44, 8");
	asm volatile("mov r3, #0x200");
	asm volatile("umull r0, r1, r2, r3");

	print64(r1, r0);

	return 0;
}
