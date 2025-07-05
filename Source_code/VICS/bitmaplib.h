#ifndef BITMAPLIB_H
#define BITMAPLIB_H

#define BF_TYPE 0x4D42             /* "MB" */
#define TWOPI           6.283185307179586476925287
#define ABS(x) (x < 0 ? -(x) : (x))

/* 18 bytes long */
typedef struct {
   char  idlength;
   char  colourmaptype;
   char  datatypecode;
   short int colourmaporigin;
   short int colourmaplength;
   char  colourmapdepth;
   short int x_origin;
   short int y_origin;
   short int width;
   short int height;
   char  bitsperpixel;
   char  imagedescriptor;
} TGAHEADER;


void WriteBitmap(char *,FILE *,int,int,int);
void write_JPEG_file (char *fName, FILE *fraw, int width, int height);
int write_JPEG2000_file(char *fName, FILE *fraw, int width, int height);

#endif /* BITMAPLIB_H */

