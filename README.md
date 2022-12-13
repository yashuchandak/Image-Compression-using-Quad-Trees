# Image-Compression-using-Quad-Trees

### We can reduce Image size by reducing image color details using this program. 

### Working: 
Input is a BMP image file (BitMap image file is an uncompressed file format which stores color details for each pixel of image). Pixel matrix of image is extracted from BMP file. Image (pixel matrix) is recursively divided into four subparts until the difference between average R,G,B and majority R,G,B value is less than threshold. The four subparts becomes the four children of quadtree node. Each node contains starting coordinates of sub-matrix, height and width of matrix, avg R, G, B values and 4 children pointers. Output BMP image is made from leaf nodes of Quadtrees. 

NOTE : No difference in size will be seen in Output BMP image file. The Output image will be Pixelized. The difference in size will be observed when both input and output BMP images are converted to other formats (jpg or png) and then compared. 

### Used:
* Programming language: C
* Data Structure: Quad Trees
 
