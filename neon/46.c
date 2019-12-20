#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <asm/types.h>

#include "arm_neon.h"
#include "ogl_sdl_support.h"

#define	MAX		10000

typedef struct timeval	tv;

static unsigned char *texels;
static unsigned int width, height;

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
	int width4 = width * 4;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width4; j += 4)
		{
			val = in[j + i * width4 + 0] * 0.299 +
				in[j + i * width4 + 1] * 0.587 +
				in[j + i * width4 + 2] * 0.144;

			out[j + i * width4 + 0] = val;
			out[j + i * width4 + 1] = val;
			out[j + i * width4 + 2] = val;
			out[j + i * width4 + 3] = in[j + i * width4 + 3];
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
	unsigned char *out;

#if 0
	gettimeofday(&start, NULL);

	general_gray_scale(texels, out, width, height);

	gettimeofday(&end, NULL);
	get_runtime(start, end);
#endif

	AppData ad = {0,};

    if (0 != SDL_Init(SDL_INIT_EVERYTHING))
	{
       SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[SDL] Initialize Failed");
       return -1;
    }

    ad.window = SDL_CreateWindow(NULL, 2000, 0, WIDTH, HEIGHT, 
							SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (ad.window == 0)
	{
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, 
						"[SDL] Failed to create Window");
        SDL_Quit();
        return -1;
    }

    surface = SDL_LoadBMP("./f22.bmp");
    initGL(&ad);
 
    GLInit(&ad);
    SDL_bool isQuit = SDL_FALSE;

    while(!isQuit)
	{
        SDL_Event event;

        if(SDL_PollEvent(&event))
		{
            switch(event.type)
            {
            case SDL_QUIT:
            isQuit = SDL_TRUE;
            break;
            }
        }
        drawGL(&ad);
        SDL_GL_SwapWindow(ad.window);
    }

    SDL_Quit();
    return 0;
}