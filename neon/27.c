#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <asm/types.h>

#include "arm_neon.h"

typedef struct timeval	tv;

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
		printf("%4d", out[i]);

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
		printf("%7d", out[i]);

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

void init_arr(int *arr, int size)
{
	int i;

	for(i = 0; i < size; i++)
		arr[i] = i + 1;
}

int get_sum_arr(int *arr, int size)
{
	int i = 0, sum = 0;

	for(i = 0; i < size; i++)
		sum += arr[i];

	return sum;
}

int neon_get_sum_arr(int *arr, int size)
{
	int remain = size % 4;
	int loop_cnt = size - remain;
	int sum[4];
	int i = 0, ret_sum = 0;
	int32x4_t neon_val, neon_res;

	neon_res = vdupq_n_s32(0);

	for(i = 0; i < loop_cnt; i += 4)
	{
		neon_val = vld1q_s32(&arr[i]);
		neon_res = vaddq_s32(neon_res, neon_val);
	}

	vst1q_s32(&sum[0], neon_res);

	for(i = 0; i < 4; i++)
		ret_sum += sum[i];

	if(remain != 0)
	{
		for(i = loop_cnt; i < size; i++)
		{
			ret_sum += sum[i];
		}
	}

	return ret_sum;
}

double get_runtime(tv start, tv end)
{
	end.tv_usec = end.tv_usec - start.tv_usec;
	end.tv_sec = end.tv_sec - start.tv_sec;
	end.tv_usec += end.tv_sec * 1000000;

	printf("runtime = %lf sec\n", end.tv_usec / 1000000.0);

	return end.tv_usec / 1000000.0;
}

int main(void)
{
	int16x8_t a, b;
	uint16x8_t r;
	uint16_t in1[8] = {0, 2, 0, 1, 1, 1, 0, 1};
	uint16_t in2[8] = {-1, 1, 0, 0, 1, 2, 1, 1};

	a = vld1q_s16(&in1[0]);
	b = vld1q_s16(&in2[0]);
	r = vceqq_s16(a, b);
	print_u16_vectors(r);

	r = vcgeq_s16(a, b);
	print_u16_vectors(r);

	r = vcleq_s16(a, b);
	print_u16_vectors(r);

	r = vcgtq_s16(a, b);
	print_u16_vectors(r);

	r = vcltq_s16(a, b);
	print_u16_vectors(r);

	return 0;
}
