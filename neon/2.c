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
	uint16x8x2_t r;
	uint16x4x2_t r2;
	uint16_t ptr[2] = {5, 6};
	uint16_t in[16] = {0, 1,  2,  3,  4,  5,  6,  7,
						8, 9, 10, 11, 12, 13, 14, 15};

	r = vld2q_u16(&in[0]);
	print_2u16x8x2_vectors(r);

	r = vld2q_lane_u16(&ptr[0], r, 1);
	print_2u16x8x2_vectors(r);

	r2 = vld2_dup_u16(&ptr[0]);
	print_2u16x4x2_vectors(r2);

	return 0;
}
