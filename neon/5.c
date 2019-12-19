#include <stdio.h>
#include <stdint.h>
#include <asm/types.h>
#include "arm_neon.h"

void print_2u16x8x2_vectors(uint16x8x2_t in)
{
	int i;
	uint16_t out[16];

	vst2q_u16(&out[0], in);

	for(i = 0; i < 16; i++)
		printf("%3d", out[i]);

	printf("\n");
}

void print_u16x8_vectors(uint16x4_t in)
{
	int i;
	uint16_t out[4];

	vst1_u16(&out[0], in);

	for(i = 0; i < 4; i++)
		printf("%3d", out[i]);

	printf("\n");
}

void print_2u16x4x2_vectors(uint16x4x2_t in)
{
	int i;
	uint16_t out[8];

	vst2_u16(&out[0], in);

	for(i = 0; i < 8; i++)
		printf("%3d", out[i]);

	printf("\n");
}

void print_u16_vectors(uint16x8_t in)
{
	int i;
	uint16_t out[8];

	vst1q_u16(&out[0], in);

	for(i = 0; i < 8; i++)
		printf("%3d", out[i]);

	printf("\n");
}

void print_u16_arr(uint16_t *arr)
{
	int i;

	for(i = 0; i < 8; i++)
		printf("%3d", arr[i]);

	printf("\n");
}

int main(void)
{
	uint16x8_t a, b, r;
	uint16_t in1[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	uint16_t in2[8] = {7, 6, 5, 4, 3, 2, 1, 0};
	uint16_t out[8];

	a = vld1q_u16(&in1[0]);
	b = vld1q_u16(&in2[0]);

	r = vmaxq_u16(a, b);
	print_u16_vectors(r);

	r = vminq_u16(a, b);
	print_u16_vectors(r);

	return 0;
}
