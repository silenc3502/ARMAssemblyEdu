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
	char *str;
	uint16x8_t a, b, r;
	uint16_t in[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	uint16_t out[8];
	int i = 0;

	a = vld1q_u16(&in[0]);
	b = vld1q_u16(&in[0]);

	r = vaddq_u16(a, b);
	print_u16_vectors(r);

	vst1q_u16(&out[0], r);
	print_u16_arr(out);

	r = vsubq_u16(a, b);
	print_u16_vectors(r);

	vst1q_u16(&out[0], r);
	print_u16_arr(out);

	r = vmulq_u16(a, b);
	print_u16_vectors(r);

	vst1q_u16(&out[0], r);
	print_u16_arr(out);

	return 0;
}
