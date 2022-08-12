#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>


typedef struct qtnode {
	struct  qtnode *ul, *ur, *bl, *br;
	int x, y;
	int len;
	int hei;
	int r, g, b;
}qtnode;


int max(int arr[]) {
	int max = arr[0];
	int maxi = 0;
	for(int i=1; i<256; i++) {
		if(arr[i] > max) {
			max = arr[i];
			maxi = i;
		}
	}
	return maxi;
}

int c = 0;
void compress(int **mat[3], int x, int y, int len, int hei, qtnode **I, int error, int maxbox) {
	int loss = error/pow(len*hei, 0.125);
	// c++;
	// if(len == 1 && hei == 1) return;
    int bk = 0;
	
	int tot[3], avg[3];
	for(int i=0; i<3; i++) {
		tot[i] = 0;
		avg[i] = 0;
	} 
	int count = 0;
	int *majority[3];
	for(int i=0; i<3; i++) {
		majority[i] = (int *)calloc(256, sizeof(int));
	}
	int maj[3];

	
		for(int j=y; j<y+hei; j++) {
			for(int i=x; i<x+len; i++) {
				count++;
				for(int k=0; k<3; k++) {
					majority[k][mat[k][j][i]] += 1;
					// count++;
					tot[k] = tot[k] + mat[k][j][i];
				}
			}
		}
	// printf("%d %d ", len, hei);
	if(count == 0) return;
	
	c++;

	for(int i=0; i<3; i++) {
		avg[i] = tot[i] / count;
		maj[i] = max(majority[i]);
	}

	if(abs(avg[0]-maj[0]) > loss || abs(avg[1]-maj[1]) > loss || abs(avg[2]-maj[2] > loss || len>maxbox || hei>maxbox)) {
		
		// printf("%d %d", len, hei);
		(*I)->ul = (qtnode *)malloc(sizeof(qtnode));
		compress(mat, x, y, len/2, hei/2, &((*I)->ul), error, maxbox);
		
		(*I)->ur = (qtnode *)malloc(sizeof(qtnode));
		compress(mat, x+len/2, y, len-len/2, hei/2, &((*I)->ur), error, maxbox);
		
		(*I)->bl = (qtnode *)malloc(sizeof(qtnode));
		compress(mat, x, y+hei/2, len/2, hei-hei/2, &((*I)->bl), error, maxbox);
		
		(*I)->br = (qtnode *)malloc(sizeof(qtnode));
		compress(mat, x+len/2, y+hei/2, len-len/2, hei-hei/2, &((*I)->br), error, maxbox);
		
		bk = 1;
	}
	
	if(bk == 0) {
		(*I)->r = avg[0];
		(*I)->g = avg[1];
		(*I)->b = avg[2];
		(*I)->bl = (*I)->br = (*I)->ul = (*I)->ur = NULL;
        free(majority[0]);
		free(majority[1]);
		free(majority[2]);
    }
	else {
		(*I)->r = (*I)->g = (*I)->b = -1;
	}
	(*I)->len = len;
	(*I)->hei = hei;
	(*I)->y = y;
	(*I)->x = x;
}


void decompress(qtnode *I, int **dmat[3]) {
	if(!I)
		return;
	if(I->r == -1) {	
		decompress(I->ul, dmat);
		decompress(I->ur, dmat);
		decompress(I->bl, dmat);
		decompress(I->br, dmat);
	}
	else {
		for(int j=I->y; j< I->y+I->hei; j++) {
			for(int i=I->x; i < I->x+I->len; i++) {
				dmat[0][j][i] = I->r;
				dmat[1][j][i] = I->g;
				dmat[2][j][i] = I->b;
				// fprintf("%d %d; ", i, j);
			}
		}
	}
    free(I);
}

void manipulateMatrix(int height, int width, int error, int maxbox) {
    int **cmat[3];
	int **dmat[3];

    FILE *fin = fopen("writebmp.txt", "r");
	for(int j=0; j<3; j++) {
		cmat[j] = (int **)malloc(sizeof(void *)*height);
	} 
		for(int i=0; i<height; i++) {
	    	cmat[0][i] = (int *)malloc(sizeof(int)*width);
			cmat[1][i] = (int *)malloc(sizeof(int)*width);
			cmat[2][i] = (int *)malloc(sizeof(int)*width);
			for(int j=0; j<width; j++) {
			    fscanf(fin, "%d %d %d", &cmat[0][i][j], &cmat[1][i][j], &cmat[2][i][j]);
	    	}
    	}
		
	qtnode *I = (qtnode *)malloc(sizeof(qtnode));

    for(int k=0; k<3; k++) {
		dmat[k] = (int **)malloc(sizeof(void *)*height);
	}
	for(int k=0; k<3; k++) {    
		for(int i=0; i<height; i++) {
		    dmat[k][i] = (int *)malloc(sizeof(int)*width);
		}
    }

	
	compress(cmat, 0, 0, width, height, &I, error*pow(width*height, 0.125), maxbox);
	printf("%d", c);
	decompress(I, dmat);
    

    for(int k=0; k<3; k++) {
	    for(int i=0; i<height; i++) {
		    free(cmat[k][i]);
	    }
    }
	
	FILE *fo = fopen("comprmatrix.txt", "w");

	for(int i=height-1; i>=0; i--) {
		for(int j=0; j<width; j++) {
			fprintf(fo, "%d %d %d ", dmat[2][i][j], dmat[1][i][j], dmat[0][i][j]);
		}
		fprintf(fo, "\n");
	}

    for(int k=0; k<3; k++) {
	    for(int i=0; i<height; i++) {
		    free(dmat[k][i]);
	    }
    }
    
	fclose(fo);
	fclose(fin);
}
