#include<stdio.h>
#include<stdlib.h>


typedef struct header1 {
    char name[2];
    unsigned int size;
    int garbage;
    unsigned int image_offset;
}header1;

typedef struct header2 {
    unsigned int header_size;
    unsigned int width;
    unsigned int height;
    unsigned short int colorplanes;
    unsigned short int bitsperpixel;
    unsigned int compression;
    unsigned int image_size;
}header2;

typedef struct pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
}pixel;

typedef struct image_Array {
    int height;
    int width;
    pixel** pixel_array;
}image_Array;


void readImage(FILE* fp, int h, int w) {
    FILE *fw = fopen("writebmp.txt","w");
    image_Array pic;
    pic.pixel_array = (pixel**)malloc(h*sizeof(void*));
    pic.height = h;
    pic.width = w;
    for(int i=h-1; i>=0; i--) {
        pic.pixel_array[i] = (pixel*)malloc(w*sizeof(pixel));
        fread(pic.pixel_array[i], w, sizeof(pixel), fp);
    }
    for(int i=0; i<h; i++) {
        for(int j=0; j<w; j++) {
            fprintf(fw, "%d %d %d ", pic.pixel_array[i][j].red, pic.pixel_array[i][j].green, pic.pixel_array[i][j].blue);
        }
    }
    fclose(fw);
}


int *readBmp(char *inputfile) {
    FILE *fp=fopen(inputfile, "rb");

    header1 header;
    header2 dib;

    fread(header.name, 2, 1, fp);
    fread(&header.size, 3*sizeof(int), 1, fp);
    
    fread(&dib, sizeof(header2), 1, fp);

    fseek(fp, header.image_offset, SEEK_SET);
    
    readImage(fp, dib.height, dib.width);

    FILE *fbmp = fopen("compressed_img.bmp", "wb");
    
    fwrite(header.name, 2, 1, fbmp);
    fwrite(&header.size, 3*sizeof(int), 1, fbmp);
    fwrite(&dib, sizeof(header2), 1, fbmp);
    fclose(fbmp);

    int *arr = (int *)malloc(sizeof(int)*3);
    arr[0] = dib.height;
    arr[1] = dib.width;
    arr[2] = header.image_offset;
    return arr;
}

void writeBmp(int h, int w, int img_offset) {
    FILE *fbmp = fopen("compressed_img.bmp", "rb+");
    fseek(fbmp, img_offset, SEEK_SET);
    
    FILE *fcompr = fopen("comprmatrix.txt", "r");

    for(int i=0; i<h; i++) {
        for(int j=0; j<3*w; j++) {
            int num;
            fscanf(fcompr, "%d", &num);
            fwrite(&num, 1, 1, fbmp);
        }
    }

    fclose(fcompr);
    fclose(fbmp); 
}
