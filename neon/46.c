#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <asm/types.h>

#include "arm_neon.h"

#define	MAX		10000

typedef struct timeval	tv;

#include <stdio.h>
#include <stdlib.h>

#pragma pack(push,1)
/* Windows 3.x bitmap file header */
typedef struct {
    char         filetype[2];   /* magic - always 'B' 'M' */
    unsigned int filesize;
    short        reserved1;
    short        reserved2;
    unsigned int dataoffset;    /* offset in bytes to actual bitmap data */
} file_header;

/* Windows 3.x bitmap full header, including file header */
typedef struct {
    file_header  fileheader;
    unsigned int headersize;
    int          width;
    int          height;
    short        planes;
    short        bitsperpixel;  /* we only support the value 24 here */
    unsigned int compression;   /* we do not support compression */
    unsigned int bitmapsize;
    int          horizontalres;
    int          verticalres;
    unsigned int numcolors;
    unsigned int importantcolors;
} bitmap_header;
#pragma pack(pop)

#if 0
#pragma pack(2)
typedef struct                       /**** BMP file header structure ****/
{
    unsigned short bfType;           /* Magic number for file */
    unsigned int   bfSize;           /* Size of file */
    unsigned short bfReserved1;      /* Reserved */
    unsigned short bfReserved2;      /* ... */
    unsigned int   bfOffBits;        /* Offset to bitmap data */
} BITMAPFILEHEADER;

typedef struct                       /**** BMP file info structure ****/
{
    unsigned int   biSize;           /* Size of info header */
    int            biWidth;          /* Width of image */
    int            biHeight;         /* Height of image */
    unsigned short biPlanes;         /* Number of color planes */
    unsigned short biBitCount;       /* Number of bits per pixel */
    unsigned int   biCompression;    /* Type of compression to use */
    unsigned int   biSizeImage;      /* Size of image data */
    int            biXPelsPerMeter;  /* X pixels per meter */
    int            biYPelsPerMeter;  /* Y pixels per meter */
    unsigned int   biClrUsed;        /* Number of colors used */
    unsigned int   biClrImportant;   /* Number of important colors */
} BITMAPINFOHEADER;

typedef struct                       /**** Colormap entry structure ****/
{
    unsigned char  rgbBlue;          /* Blue value */
    unsigned char  rgbGreen;         /* Green value */
    unsigned char  rgbRed;           /* Red value */
    unsigned char  rgbReserved;      /* Reserved */
} RGBQUAD;

typedef struct _bitmap {
	void (* bitmap)(const char *fn, bitmap *this);
	RGBQUAD *pixels;
	BITMAPFILEHEADER fh;
    BITMAPINFOHEADER ih;
} bitmap;
#endif

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
		printf("%6d", out[i]);

	printf("\n");
}

void print_f32_vectors(float32x4_t in)
{
	int i;
	float32_t out[4];

	vst1q_f32(&out[0], in);

	for(i = 0; i < 4; i++)
		printf("%8.3f", out[i]);

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

float general_std_dev(float *src, int size)
{
	int i;
	float sum = 0, sum_dist = 0, avg, dist_val;

	for(i = 0; i < size; i++)
		sum += src[i];

	avg = sum / size;

	for(i = 0; i < size; i++)
	{
		dist_val = avg - src[i];
		sum_dist += dist_val * dist_val;
	}

	return sqrt(sum_dist / size);
}

float neon_std_dev(float *src, int size)
{
	int i;
	int loop = (size / 4) * 4;
	float sum[4];
	float total = 0, result = 0, avg;
	float32x4_t neon_sum, cur_val, res_val;
	float32x4_t neon_avg, dist, sum_dist;

	neon_sum = vdupq_n_f32(0);
	cur_val = vdupq_n_f32(0);
	res_val = vdupq_n_f32(0);

	for(i = 0; i < loop; i += 4)
	{
		cur_val = vld1q_f32(src + i);
		neon_sum = vaddq_f32(neon_sum, cur_val);
	}

	vst1q_f32(&sum[0], neon_sum);

	for(i = 0; i < 4; i++)
		total += sum[i];

	for(i = loop; i < size; i++)
		total += src[i];

	avg = total / size;
	neon_avg = vdupq_n_f32(avg);
	dist = vdupq_n_f32(0);
	sum_dist = vdupq_n_f32(0);;

	for(i = 0; i < loop; i += 4)
	{
		cur_val = vld1q_f32(src + i);
		dist = vsubq_f32(neon_avg, cur_val);
		dist = vmulq_f32(dist, dist);
		sum_dist = vaddq_f32(sum_dist, dist);
	}

	vst1q_f32(&sum[0], sum_dist);
	total = 0;

	for(i = 0; i < 4; i++)
		total += sum[i];

	for(i = loop; i < size; i++)
		total += (avg - src[i]) * (avg - src[i]);

	return sqrt(total / size);
}

void general_4x4_matrix_mul(float *A, float *B, float *R)
{
	R[0]  = A[0]  * B[0] + A[1]  * B[4] + A[2]  * B[8]  + A[3]  * B[12];
	R[1]  = A[0]  * B[1] + A[1]  * B[5] + A[2]  * B[9]  + A[3]  * B[13];
	R[2]  = A[0]  * B[2] + A[1]  * B[6] + A[2]  * B[10] + A[3]  * B[14];
	R[3]  = A[0]  * B[3] + A[1]  * B[7] + A[2]  * B[11] + A[3]  * B[15];

	R[4]  = A[4]  * B[0] + A[5]  * B[4] + A[6]  * B[8]  + A[7]  * B[12];
	R[5]  = A[4]  * B[1] + A[5]  * B[5] + A[6]  * B[9]  + A[7]  * B[13];
	R[6]  = A[4]  * B[2] + A[5]  * B[6] + A[6]  * B[10] + A[7]  * B[14];
	R[7]  = A[4]  * B[3] + A[5]  * B[7] + A[6]  * B[11] + A[7]  * B[15];

	R[8]  = A[8]  * B[0] + A[9]  * B[4] + A[10] * B[8]  + A[11] * B[12];
	R[9]  = A[8]  * B[1] + A[9]  * B[5] + A[10] * B[9]  + A[11] * B[13];
	R[10] = A[8]  * B[2] + A[9]  * B[6] + A[10] * B[10] + A[11] * B[14];
	R[11] = A[8]  * B[3] + A[9]  * B[7] + A[10] * B[11] + A[11] * B[15];

	R[12] = A[12] * B[0] + A[13] * B[4] + A[14] * B[8]  + A[15] * B[12];
	R[13] = A[12] * B[1] + A[13] * B[5] + A[14] * B[9]  + A[15] * B[13];
	R[14] = A[12] * B[2] + A[13] * B[6] + A[14] * B[10] + A[15] * B[14];
	R[15] = A[12] * B[3] + A[13] * B[7] + A[14] * B[11] + A[15] * B[15];
}

void neon_4x4_matrix_mul(float *A, float *B, float *R)
{
	int i;
	int idx1 = 0, idx2;
	float32x4_t neon_r;
	float32x4_t neon_b[4];

	for(i = 0; i < 4; i++)
		neon_b[i] = vld1q_f32((B + (i * 4)));

	for(i = 0; i < 4; i++)
	{
		idx2 = 0;
		neon_r = vaddq_f32(vmulq_f32(vdupq_n_f32(A[idx1++]), neon_b[idx2++]),
				 vaddq_f32(vmulq_f32(vdupq_n_f32(A[idx1++]), neon_b[idx2++]),
				 vaddq_f32(vmulq_f32(vdupq_n_f32(A[idx1++]), neon_b[idx2++]),
				 vmulq_f32(vdupq_n_f32(A[idx1++]), neon_b[idx2++]))));
		vst1q_f32(R + (i * 4), neon_r);
	}
}

void neon_4x4_matrix_mul2(float *A, float32x4_t *B, float *R)
{
	int i;
	int idx1 = 0, idx2;
	float32x4_t neon_r;
	float32x4_t neon_b[4];

	for(i = 0; i < 4; i++)
	{
		idx2 = 0;
		neon_r = vaddq_f32(vmulq_f32(vdupq_n_f32(A[idx1++]), neon_b[idx2++]),
				 vaddq_f32(vmulq_f32(vdupq_n_f32(A[idx1++]), neon_b[idx2++]),
				 vaddq_f32(vmulq_f32(vdupq_n_f32(A[idx1++]), neon_b[idx2++]),
				 vmulq_f32(vdupq_n_f32(A[idx1++]), neon_b[idx2++]))));
		vst1q_f32(R + (i * 4), neon_r);
	}
}

void general_gray_scale(unsigned char *in, unsigned char *out, int width, int height)
{
	int i, j, val = 0;
	int width4 = width * 3;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width4; j += 3)
		{
			val = in[j + i * width4 + 0] * 0.299 +
				in[j + i * width4 + 1] * 0.587 +
				in[j + i * width4 + 2] * 0.144;

			out[j + i * width4 + 0] = val;
			out[j + i * width4 + 1] = val;
			out[j + i * width4 + 2] = val;
			//out[j + i * width4 + 3] = in[j + i * width4 + 3];
		}
	}
}

void neon_gray_scale(unsigned char *in, unsigned char *out, int width, int height)
{
	int j = 0, i = 0;
	int width4 = width * 3;

	float32x4_t redoffset = vmovq_n_f32(0.299);
	float32x4_t greenoffset = vmovq_n_f32(0.587);
	float32x4_t blueoffset = vmovq_n_f32(0.114);

	uint8x8x3_t rgb, dest;
	uint16x8_t red, green, blue;
	uint16x4_t high, low;
	uint32x4_t inthi, intlo;
	float32x4_t floathi, floatlo;
	uint8x8_t TotalValue, greenvalue, bluevalue;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width4; j += 24)
		{
			rgb = vld3_u8(&in[j + i * width4]);

			red = vmovl_u8(rgb.val[0]);
			high = vget_high_u16(red);
			low = vget_low_u16(red);
			inthi = vmovl_u16(high);
			intlo = vmovl_u16(low);
			floathi = vcvtq_f32_u32(inthi);
			floatlo = vcvtq_f32_u32(intlo);
			floathi = vmulq_f32(floathi, redoffset);
			floatlo = vmulq_f32(floatlo, redoffset);
			inthi = vcvtq_u32_f32(floathi);
			intlo = vcvtq_u32_f32(floatlo);
			high = vmovn_u32(inthi);
			low = vmovn_u32(intlo);
			red = vcombine_u16(low, high);
			TotalValue = vmovn_u16(red);

			green = vmovl_u8(rgb.val[1]);
			high = vget_high_u16(green);
			low = vget_low_u16(green);
			inthi = vmovl_u16(high);
			intlo = vmovl_u16(low);
			floathi = vcvtq_f32_u32(inthi);
			floatlo = vcvtq_f32_u32(intlo);
			floathi = vmulq_f32(floathi, greenoffset);
			floatlo = vmulq_f32(floatlo, greenoffset);
			inthi = vcvtq_u32_f32(floathi);
			intlo = vcvtq_u32_f32(floatlo);
			high = vmovn_u32(inthi);
			low = vmovn_u32(intlo);
			green = vcombine_u16(low, high);
			greenvalue = vmovn_u16(green);

			TotalValue = vadd_u8(TotalValue, greenvalue);

			blue = vmovl_u8(rgb.val[2]);
			high = vget_high_u16(blue);
			low = vget_low_u16(blue);
			inthi = vmovl_u16(high);
			intlo = vmovl_u16(low);
			floathi = vcvtq_f32_u32(inthi);
			floatlo = vcvtq_f32_u32(intlo);
			floathi = vmulq_f32(floathi, blueoffset);
			floatlo = vmulq_f32(floatlo, blueoffset);
			inthi = vcvtq_u32_f32(floathi);
			intlo = vcvtq_u32_f32(floatlo);
			high = vmovn_u32(inthi);
			low = vmovn_u32(intlo);
			blue = vcombine_u16(low, high);
			bluevalue = vmovn_u16(blue);

			TotalValue = vadd_u8(TotalValue, bluevalue);

			dest.val[0] = TotalValue;
			dest.val[1] = TotalValue;
			dest.val[2] = TotalValue;
			//dest.val[3] = rgb.val[3];

			vst3_u8(&out[j + i * width4], dest);
		}
	}
}

#if 0
void bitmap(const char *fn, bitmap *this)
{
	FILE *file;
	file = fopen(fn, "rb");

	if(file != NULL)
	{
		BITMAPFILEHEADER h;

		//reading the FILEHEADER
        size_t x = fread(&h, sizeof(BITMAPFILEHEADER), 1, file); 

        std::cout << x;
        fread(this->ih, sizeof(BITMAPINFOHEADER), 1, file);

        fclose(file);
	}
}

static void readBmp(char *filename)
{
		FILE *fd;

		fd = fopen(filename, "rb");
		if (fd == NULL)
		{
				printf("Error: fopen failed\n");
				return;
		}

		unsigned char header[54];

		// Read header
		fread(header, sizeof(unsigned char), 54, fd);

		// Capture dimensions
		width = *(int*)&header[18];
		height = *(int*)&header[22];

		int padding = 0;

		// Calculate padding
		while ((width * 3 + padding) % 4 != 0)
		{
				padding++;
		}

		// Compute new width, which includes padding
		int widthnew = width * 3 + padding;

		// Allocate memory to store image data (non-padded)
		texels = (unsigned char *)malloc(width * height * 3 * sizeof(unsigned char));
		if (texels == NULL)
		{
				printf("Error: Malloc failed\n");
				return;
		}

		// Allocate temporary memory to read widthnew size of data
		unsigned char* data = (unsigned char *)malloc(widthnew * sizeof (unsigned int));

		// Read row by row of data and remove padded data.
		for (int i = 0; i<height; i++)
		{
				// Read widthnew length of data
				fread(data, sizeof(unsigned char), widthnew, fd);

				// Retain width length of data, and swizzle RB component.
				// BMP stores in BGR format, my usecase needs RGB format
				for (int j = 0; j < width * 3; j += 3)
				{
						int index = (i * width * 3) + (j);
						texels[index + 0] = data[j + 2];
						texels[index + 1] = data[j + 1];
						texels[index + 2] = data[j + 0];
				}
		}

		free(data);
		fclose(fd);
}
#endif

int main(int argc, char **argv)
{
	tv start, end;

	int n;
	char *data_in;
	char *data_out;
	char *neon_data_out;
	FILE *in, *out, *neon_out;
	bitmap_header *hp;

	in = fopen("./f22.bmp", "rb");
	if(in == NULL)
	{
		printf("Fail to open input image\n");
		exit(-1);
	}

	hp = (bitmap_header *)malloc(sizeof(bitmap_header));
	if(hp == NULL)
	{
		printf("Fail to Memory Allocation\n");
		exit(-1);
	}

	printf("Bits per Pixel = %u\n", hp->bitsperpixel);

	n = fread(hp, sizeof(bitmap_header), 1, in);
	if(n < 1) 
	{
		printf("Something Wrong when read header\n");
		exit(-1);
	}

	data_in = (char *)malloc(sizeof(char) * hp->bitmapsize);
	if(data_in == NULL)
	{
		printf("Fail to Allocate Input Image Data\n");
		exit(-1);
	}

#if 0
	data_out = (char *)malloc(sizeof(char) * hp->bitmapsize * 4 / 3);
#else
	data_out = (char *)malloc(sizeof(char) * hp->bitmapsize);
#endif
	if(data_out == NULL)
	{
		printf("Fail to Allocate Output Image Data\n");
		exit(-1);
	}

	neon_data_out = (char *)malloc(sizeof(char) * hp->bitmapsize);
	if(neon_data_out == NULL)
	{
		printf("Fail to Allocate Neon Output Image Data\n");
		exit(-1);
	}

	fseek(in, sizeof(char) * hp->fileheader.dataoffset, SEEK_SET);
	n = fread(data_in, sizeof(char), hp->bitmapsize, in);
	if(n < 1)
	{
		printf("Something Wrong when read data\n");
		exit(-1);
	}

	out = fopen("res.bmp", "wb");
	if(out == NULL)
	{
		printf("Fail to open output image\n");
		exit(-1);
	}

	neon_out = fopen("neon_res.bmp", "wb");
	if(out == NULL)
	{
		printf("Fail to open neon output image\n");
		exit(-1);
	}


	gettimeofday(&start, NULL);

	general_gray_scale(data_in, data_out, hp->width, hp->height);

	gettimeofday(&end, NULL);
	get_runtime(start, end);

	n = fwrite(hp, sizeof(char), sizeof(bitmap_header), out);
	if(n < 1)
	{
		printf("write data: header\n");
		exit(-1);
	}

	fseek(out, sizeof(char) * hp->fileheader.dataoffset, SEEK_SET);
#if 0
	n = fwrite(data_out, sizeof(char), hp->bitmapsize * 4 / 3, out);
#else
	n = fwrite(data_out, sizeof(char), hp->bitmapsize, out);
#endif
	if(n < 1)
	{
		printf("write data: image data\n");
		exit(-1);
	}

	gettimeofday(&start, NULL);

	neon_gray_scale(data_in, neon_data_out, hp->width, hp->height);

	gettimeofday(&end, NULL);
	get_runtime(start, end);

	n = fwrite(hp, sizeof(char), sizeof(bitmap_header), neon_out);
	if(n < 1)
	{
		printf("neon write data: header\n");
		exit(-1);
	}

	fseek(neon_out, sizeof(char) * hp->fileheader.dataoffset, SEEK_SET);
	n = fwrite(neon_data_out, sizeof(char), hp->bitmapsize, neon_out);
	if(n < 1)
	{
		printf("write data: image data\n");
		exit(-1);
	}

	fclose(in);
	fclose(out);
	fclose(neon_out);
	free(hp);
	free(data_in);
	free(data_out);
	free(neon_data_out);

    return 0;
}
