#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <asm/types.h>

#include "arm_neon.h"

#define	MAX		10000

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
		printf("%6d", out[i]);

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

void print_s16_vectors(int16x8_t in)
{
	int i;
	int16_t out[8];

	vst1q_s16(&out[0], in);

	for(i = 0; i < 8; i++)
		printf("%6d", out[i]);

	printf("\n");
}

void print_s32_vectors(int32x4_t in)
{
	int i;
	int32_t out[4];

	vst1q_s32(&out[0], in);

	for(i = 0; i < 4; i++)
		printf("%6d", out[i]);

	printf("\n");
}

void print_s16x4_vectors(int16x4_t in)
{
	int i;
	int16_t out[4];

	vst1_s16(&out[0], in);

	for(i = 0; i < 4; i++)
		printf("%4d", out[i]);

	printf("\n");
}

void print_f32_vectors(float32x4_t in)
{
	int i;
	float32_t out[4];

	vst1q_f32(&out[0], in);

	for(i = 0; i < 4; i++)
		printf("%6.3f", out[i]);

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

void print_u32_vectors(uint32x4_t in)
{
	int i;
	uint32_t out[4];

	vst1q_u32(&out[0], in);

	for(i = 0; i < 4; i++)
		printf("%#-11x", out[i]);

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

void general_select(int16_t *src, int size, int val)
{
	int i;

	for(i = 0; i < size; i++)
	{
		if(src[i] < val)
			src[i] = 0;
	}
}

void neon_select(int16_t *src, int size, int val)
{
	int i;
	int remain = size % 8;
	int16x8_t cur_val, res_val;
	uint16x8_t select_val;
	int16x8_t cmp_val = vdupq_n_s16(val);

	for(i = 0; i < size; i += 8)
	{
		cur_val = vld1q_s16(src + i);
		select_val = vcgtq_s16(cur_val, cmp_val);
		res_val = vreinterpretq_s16_u16(select_val);
		cur_val = vandq_s16(cur_val, res_val);
		vst1q_s16(src + i, cur_val);
	}

	if(remain != 0)
	{
		for(i = size - remain; i < size; i++)
		{
			if(src[i] < val)
				src[i] = 0;
		}
	}
}

void general_abs(short *src, int size)
{
	int i = 0;

	for(i = 0; i < size; i++)
	{
		if(src[i] < 0)
			src[i] *= -1;
	}
}

void neon_abs(short *src, int size)
{
	int i = 0;
	int remain = size % 8;
	int16x8_t cur_val;

	for(i = 0; i < size; i += 8)
	{
		cur_val = vld1q_s16(src + i);
		cur_val = vabsq_s16(cur_val);
		vst1q_s16(src + i, cur_val);
	}

	if(remain != 0)
	{
		for(i = size - remain; i < size; i++)
		{
			if(src[i] < 0)
				src[i] *= -1;
		}
	}
}

short general_max(short *src, int size)
{
	short max = 0;
	int i;

	for(i = 0; i < size; i++)
	{
		if(src[i] > max)
			max = src[i];
	}

	return max;
}

short neon_max(short *src, int size)
{
	int i;
	int remain = size % 8;
	int loop_cnt = size - remain;

	short max = 0;
	short max_arr[8];

	int16x8_t cur_val, res_val;

	res_val = vdupq_n_s16(0);

	for(i = 0; i < loop_cnt; i += 8)
	{
		cur_val = vld1q_s16(&src[i]);
		res_val = vmaxq_s16(cur_val, res_val);
	}

	vst1q_s16(&max_arr[0], res_val);

	for(i = 0; i < 8; i++)
	{
		if(max_arr[i] > max)
			max = max_arr[i];
	}

	if(remain)
	{
		for(i = loop_cnt; i < size; i++)
		{
			if(src[i] > max)
				max = src[i];
		}
	}
}

int main(void)
{
	float32_t f_in[4] = {1.0, 2.0, 3.0, 4.0};
	float32x4_t a;
	int16x8_t r;

	int16_t n_in[4] = {1, 2, 3, -4};
	int16x4_t b;
	uint16x4_t r2;
	int8x16_t c;

	a = vld1q_f32(&f_in[0]);
	r = vreinterpretq_s16_f32(a);
	print_s16_vectors(r);

	b = vld1_s16(&n_in[0]);
	r2 = vreinterpret_u16_s16(b);
	print_u16x8_vectors(r2);

	return 0;
}
