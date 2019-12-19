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
		printf("%5d", out[i]);

	printf("\n");
}

void print_s8_vectors(int8x8_t in)
{
	int i;
	int8_t out[8];

	vst1_s8(&out[0], in);

	for(i = 0; i < 8; i++)
		printf("%4d", out[i]);

	printf("\n");
}

void print_u8_vectors(uint8x8_t in)
{
	int i;
	uint8_t out[8];

	vst1_u8(&out[0], in);

	for(i = 0; i < 8; i++)
		printf("%4d", out[i]);

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
	int i;

	uint8x8_t l_a, l_b;
	uint16x8_t l_r;
	uint16x8_t a_a, a_b, a_c, a_r;
	uint16_t out1[8];
	uint16_t out2[8];

	l_a = vdup_n_u8(255);
	l_b = vdup_n_u8(10);

	l_r = vmull_u8(l_a, l_b);
	print_u16_vectors(l_r);

	a_a = vdupq_n_u16(3);
	a_b = vdupq_n_u16(20);
	a_c = vdupq_n_u16(5);

	a_r = vmlaq_u16(a_a, a_b, a_c);
	print_u16_vectors(a_r);

	a_a = vdupq_n_u16(101);
	a_r = vmlsq_u16(a_a, a_b, a_c);
	print_u16_vectors(a_r);

	return 0;
}
