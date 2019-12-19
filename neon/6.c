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
		printf("%4d", out[i]);

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

	// long add value
	uint8x8_t l_a, l_b;
	uint16x8_t l_r;
	uint16_t l_out[8];

	// wide add value
	uint16x8_t w_a, w_r;
	uint8x8_t w_b;
	uint16_t w_out[8];

	// saturation add value
	int8x8_t q_a, q_b, q_r;
	int8_t q_in[8];
	int8_t q_out[8];

	// narrow add value
	uint16x8_t n_a, n_b;
	uint8x8_t n_r;
	uint16_t n_in[8];
	uint16_t n_in2[8];
	uint8_t n_out[8];

	// long add
	l_a = vdup_n_u8(255);
	l_b = vdup_n_u8(255);
	l_r = vaddl_u8(l_a, l_b);
	print_u16_vectors(l_r);

	// wide add
	w_a = vdupq_n_u16(255);
	w_b = vdup_n_u8(255);
	w_r = vaddw_u8(w_a, w_b);
	print_u16_vectors(w_r);

	// saturation add
	for(i = 0; i < 8; i++)
	{
		if(i < 4)
		{
			q_in[i] = -128;
		}
		else
		{
			q_in[i] = 127;
		}
	}

	q_a = vld1_s8(&q_in[0]);
	q_b = vld1_s8(&q_in[0]);
	q_r = vqadd_s8(q_a, q_b);
	print_s8_vectors(q_r);

	// naroow add
	for(i = 0; i < 8; i++)
	{
		n_in[i] = 155;
		n_in2[i] = 101;
	}

	n_a = vld1q_u16(&n_in[0]);
	n_b = vld1q_u16(&n_in2[0]);
	n_r = vaddhn_u16(n_a, n_b);
	print_u8_vectors(n_r);

	return 0;
}
