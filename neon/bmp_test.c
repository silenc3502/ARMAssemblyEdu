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

int bmp_process(char *input, char *output) {

    //variable dec:
    FILE *fp,*out;
    bitmap_header* hp;
    int n;
    char *data;

    //Open input file:
    fp = fopen(input, "rb");
    if(fp==NULL){
        //cleanup
    }


    //Read the input file headers:
    hp=(bitmap_header*)malloc(sizeof(bitmap_header));
    if(hp==NULL)
        return 3;

    n=fread(hp, sizeof(bitmap_header), 1, fp);
    if(n<1){
        //cleanup
    }

    //Read the data of the image:
    data = (char*)malloc(sizeof(char)*hp->bitmapsize);
    if(data==NULL){
        //cleanup
    }

    fseek(fp,sizeof(char)*hp->fileheader.dataoffset,SEEK_SET);
    n=fread(data,sizeof(char),hp->bitmapsize, fp);
    if(n<1){
        //cleanup
    }

        //Open output file:
    out = fopen(output, "wb");
    if(out==NULL){
        //cleanup
    }

    n=fwrite(hp,sizeof(char),sizeof(bitmap_header),out);
    if(n<1){
        //cleanup
    }
    fseek(out,sizeof(char)*hp->fileheader.dataoffset,SEEK_SET);
    n=fwrite(data,sizeof(char),hp->bitmapsize,out);
    if(n<1){
        //cleanup
    }

    fclose(fp);
    fclose(out);
    free(hp);
    free(data);
    return 0;
}

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		printf("Something Wrong\n");
		exit(-1);
	}

	if(!bmp_process(argv[1], argv[2]))
	{
		printf("Success\n");
	}
	else
	{
		printf("Fail\n");
	}
}
