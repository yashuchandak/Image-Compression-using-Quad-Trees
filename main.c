#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

int *readBmp(char *inputfile);
void manipulateMatrix(int height , int width, int error, int maxbox);
void writeBmp(int h, int w, int img_offset);

int main(int argc, char *argv[]) {
    
    char *inputfile = (char *)malloc(sizeof(char)*50);
    inputfile = argv[1];

    int *arr;

    arr = readBmp(inputfile);

    // printf("%d %d %d", arr[0], arr[1], arr[2]);

    int error = atoi(argv[2]);
    
    int maxbox;
	if(argv[3]) {
		maxbox = atoi(argv[3]);
	}
	else {
		maxbox = INT_MAX;
	}
    // printf("sdfs");
    manipulateMatrix(arr[0], arr[1], error, maxbox);

    writeBmp(arr[0], arr[1], arr[2]);

    return 0;
}