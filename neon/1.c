#include <stdio.h>
#include <stdint.h>
#include <asm/types.h>
#include "arm_neon.h"

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
	int i = 0;
	uint16x8_t r;
	uint16_t in[8];
	uint16_t ptr = 5;

	for(i = 0; i < 8; i++)
		in[i] = i;

	print_u16_arr(in);

	r = vld1q_u16(&in[0]);
	print_u16_vectors(r);

	r = vld1q_lane_u16(&ptr, r, 2);
	print_u16_vectors(r);

	r = vld1q_dup_u16(&ptr);
	print_u16_vectors(r);

	return 0;
}
