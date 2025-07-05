#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <GL/jpeglib.h>
#include <jasper/jasper.h>
#include "bitmaplib.h"


#define OPTSMAX	4096
//-----------------------------------------------------------//
// Types.
//-----------------------------------------------------------//
typedef struct {

	char *infile;	//--- The input image file.
	int infmt;      //--- The input image file format.
	char *inopts;
	char inoptsbuf[OPTSMAX + 1];
	char *outfile;  //--- The output image file.
	int outfmt;     //--- The output image file format.
	char *outopts;
	char outoptsbuf[OPTSMAX + 1];
} cmdopts_t;
//-----------------------------------------------------------//


void BM_WriteLongInt(FILE *fptr,char *s,long n)
{
	int i;

	s[0] = (n & 0xff000000) / 16777216;
	s[1] = (n & 0x00ff0000) / 65536;
	s[2] = (n & 0x0000ff00) / 256;
	s[3] = (n & 0x000000ff);
	for (i=0;i<4;i++) putc(s[i],fptr);
}

void BM_WriteHexString(FILE *fptr,char *s)
{
	int i,c;
	char hex[3];

	for (i=0;i<(int)strlen(s);i+=2) {
		hex[0] = s[i];
		hex[1] = s[i+1];
		hex[2] = '\0';
		sscanf(hex,"%X",&c);
		putc(c,fptr);
	}
}

/**************************************************************************/
/* write_word()    - Write a 16-bit unsigned integer.                     */
/* ---------------------------------------------------------------------- */
/* Returns 0 on success or -1 on failure...                               */
/* fp        - File to write to                                           */
/* w         - Integer to write                                           */
/**************************************************************************/
static int write_word(FILE *fp, unsigned short w)
{
	putc(w, fp);
    return (putc(w >> 8, fp));
}

/**************************************************************************/
/* write_dword()  -  Write a 32-bit unsigned integer.                     */
/* ---------------------------------------------------------------------- */
/* Returns 0 on success or -1 on failure...                               */
/* fp        - File to write to                                           */
/* dw        - Integer to write                                           */
/**************************************************************************/
static int write_dword(FILE *fp, unsigned int dw)
{
	putc(dw, fp);
    putc(dw >> 8, fp);
    putc(dw >> 16, fp);
    return (putc(dw >> 24, fp));
}

/**************************************************************************/
/* write_long()   -  Write a 32-bit signed integer.                       */
/* ---------------------------------------------------------------------- */
/* Returns 0 on success or -1 on failure...                               */
/* fp        - File to write to                                           */
/* l         - Integer to write                                           */
/**************************************************************************/
static int write_long(FILE *fp, int  l)
{
	putc(l, fp);
    putc(l >> 8, fp);
    putc(l >> 16, fp);
    return (putc(l >> 24, fp));
}



//-------------------------------------------------------------//
//	Write a bitmap to a file                                   //
//	The format is as follows                                   //
//		 0 == BMP                                              //
//		 1 == EPS colour (Encapsulated PostScript)             //
//		 2 == ppm                                              //
//		 3 == raw                                              //
//		 4 == rgb                                              //
//		 5 == tga                                              //
//		 6 == tiff                                             //
//.............................................................//
//	A negative format indicates a vertical flip
//-------------------------------------------------------------//

void WriteBitmap(char *fName, FILE *fraw, int nx,int ny,int format)
{
	int offset;
	int linelength = 0,size;
	char buffer[1024];
	long shift;
	unsigned char *bm;
	FILE *fptr;//, *fraw;
	long bitsize,i,j,index,width;

	if (format > 3) format -= 2;

	fptr = fopen(fName, "wb");

/* Write the header */
	switch (ABS(format)) {
//---------------------------------------------- BMP format
	case 0:
		width   = nx * 3;     /* Real width of scanline */
		width   = (width + 3) & ~3;    /* Aligned to 4 bytes */
		bitsize = width * ny; /* Size of bitmap, aligned */

		width   = nx * 3;     /* Real width of scanline */
		width   = (width + 3) & ~3;    /* Aligned to 4 bytes */
		bitsize = width * ny; /* Size of bitmap, aligned */

		/* Header 10 bytes */
		write_word(fptr, BF_TYPE);
		size = bitsize + 14 + 40;
		write_dword(fptr, size);
		write_word(fptr, 0);
		write_word(fptr, 0);
		/* Offset to image data */
		write_dword(fptr, 14+40);
		/* Information header 40 bytes */
		write_dword(fptr, 40);
		write_long(fptr, nx);
		write_long(fptr, ny);
		write_word(fptr, 1);
		write_word(fptr, 24);
		/* Compression type == 0 */
		write_dword(fptr, 0);
		write_dword(fptr, bitsize);
		write_long(fptr, 2952);
		write_long(fptr, 2952);
		write_dword(fptr, 0);
		write_dword(fptr, 0);
		putc(1,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr); 
		putc(1,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr); 
		putc(0,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr); /* No palette */
		putc(0,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr); 
		break;
//---------------------------------------------- EPS format (color)
	case 1:
		fprintf(fptr,"%%!PS-Adobe-3.0 EPSF-3.0\n");
		fprintf(fptr,"%%%%Creator: Created from bitmaplib by Paul Bourke\n");
		fprintf(fptr,"%%%%BoundingBox: %d %d %d %d\n",0,0,nx,ny);
		fprintf(fptr,"%%%%LanguageLevel: 2\n");
		fprintf(fptr,"%%%%Pages: 1\n");
		fprintf(fptr,"%%%%DocumentData: Clean7Bit\n");
		fprintf(fptr,"%d %d scale\n",nx,ny);
		fprintf(fptr,"%d %d 8 [%d 0 0 -%d 0 %d]\n",nx,ny,nx,ny,ny);
		fprintf(fptr,"{currentfile 3 %d mul string readhexstring pop} bind\n",nx);
		fprintf(fptr,"false 3 colorimage\n");
		break;
//---------------------------------------------- PPM format
	case 2:
		fprintf(fptr,"P6\n%d %d\n255\n",nx,ny);
		break;
//---------------------------------------------- RAW format
	case 3:
		break;
//---------------------------------------------- RGB format
	case 4:
		putc(0x01,fptr);
		putc(0xda,fptr);
		putc(0x00,fptr);
		putc(0x01,fptr);
		putc(0x00,fptr);
		putc(0x03,fptr);
		putc((nx & 0xFF00) / 256,fptr);
		putc((nx & 0x00FF),fptr);
		putc((ny & 0xFF00) / 256,fptr);
		putc((ny & 0x00FF),fptr);
		BM_WriteHexString(fptr,"000300000000000000ff00000000");
		fprintf(fptr,"WriteBitmap, pdb");
		putc(0x00,fptr);
		putc(0x00,fptr);
		putc(0x00,fptr);
		putc(0x00,fptr);
		putc(0x00,fptr);
		putc(0x00,fptr);
		putc(0x00,fptr);
		putc(0x00,fptr);
		break;
//---------------------------------------------- TGA format
	case 5:
		putc(0,fptr);  /* Length of ID */
		putc(0,fptr);  /* No colour map */
		putc(2,fptr);  /* uncompressed RGB  */ 
		putc(0,fptr); /* Index of colour map entry */
		putc(0,fptr);
		putc(0,fptr); /* Colour map length */
		putc(0,fptr);
		putc(0,fptr); /* Colour map size */
		putc(0,fptr); /* X origin */
		putc(0,fptr);
		putc(0,fptr); /* Y origin */
		putc(0,fptr);
		putc((nx & 0x00ff),fptr); /* X width */
		putc((nx & 0xff00) / 256,fptr);
		putc((ny & 0x00ff),fptr); /* Y width */
		putc((ny & 0xff00) / 256,fptr);
		putc(24,fptr);                 		/* 24 bit bitmap */
		putc(0x00,fptr);
		break;
//---------------------------------------------- TIFF format
	case 6:
		BM_WriteHexString(fptr,"4d4d002a");	/* Little endian & TIFF identifier */
		offset = nx * ny * 3 + 8;
		BM_WriteLongInt(fptr,buffer,offset);
		break;
		}
		
/* Write the binary data */
	bitsize = 3 * nx;
	for (j=0; j<ny; j++){
		bm = malloc(bitsize);
		if ((format != 0) && (format != 5)) {
			shift = ((ny - 1) - j) * bitsize;
			fseek( fraw, shift, SEEK_SET);
		}
		fread(bm,1,bitsize,fraw);
		for(i=0; i<nx; i++){
			index =3*i;
			switch (format) {
//---------------------------------------------- BMP format
				case 0:
					putc(bm[index+1],fptr);
					putc(bm[index+0],fptr);
					putc(bm[index+2],fptr);
					break;
//---------------------------------------------- EPS format (color)
				case 1:
					fprintf(fptr,"%02x%02x%02x",bm[index+0],bm[index+1],bm[index+2]);
					linelength += 6;
					if (linelength >= 72 || linelength >= nx) {
						fprintf(fptr,"\n");
						linelength = 0;
					}
					break;
//---------------------------------------------- TGA format
				case 5:
					putc(bm[index+2],fptr);
					putc(bm[index+1],fptr);
					putc(bm[index+0],fptr);
					break;
//---------------------------------------------- PPM format
//---------------------------------------------- RGB format
//---------------------------------------------- TIFF format
//---------------------------------------------- RAW format
				case 2:
				case 3:
				case 4:
				case 6:
					putc(bm[index+0],fptr);
					putc(bm[index+1],fptr);
					putc(bm[index+2],fptr);
					break;
			}
		}
		free(bm);
	}

/* Write the footer */
    switch (format) {
//---------------------------------------------- BMP format
//---------------------------------------------- RAW format
//---------------------------------------------- PPM format
//---------------------------------------------- RGB format
//---------------------------------------------- TGA format
	case 0:
	case 2:
	case 3:
	case 4:
	case 5:
		break;
//---------------------------------------------- TIFF format
	case 6:
		putc(0x00,fptr); /* The number of directory entries (14) */
		putc(0x0e,fptr);
		/* Width tag, short int */
		BM_WriteHexString(fptr,"0100000300000001");
		putc((nx & 0xff00) / 256,fptr);		/* Image width */
		putc((nx & 0x00ff),fptr);
		putc(0x00,fptr);
		putc(0x00,fptr);
		/* Height tag, short int */
		BM_WriteHexString(fptr,"0101000300000001");
		putc((ny & 0xff00) / 256,fptr);    /* Image height */
		putc((ny & 0x00ff),fptr);
		putc(0x00,fptr);
		putc(0x00,fptr);
		/* bits per sample tag, short int */
		BM_WriteHexString(fptr,"0102000300000003");
		offset = nx * ny * 3 + 182;
		BM_WriteLongInt(fptr,buffer,offset);
		/* Compression flag, short int */
		BM_WriteHexString(fptr,"010300030000000100010000");
		/* Photometric interpolation tag, short int */
		BM_WriteHexString(fptr,"010600030000000100020000");
		/* Strip offset tag, long int */
		BM_WriteHexString(fptr,"011100040000000100000008");
		/* Orientation flag, short int */
		BM_WriteHexString(fptr,"011200030000000100010000");
		/* Sample per pixel tag, short int */
		BM_WriteHexString(fptr,"011500030000000100030000");
		/* Rows per strip tag, short int */
		BM_WriteHexString(fptr,"0116000300000001");
		putc((ny & 0xff00) / 256,fptr); 
		putc((ny & 0x00ff),fptr);
		putc(0x00,fptr);
		putc(0x00,fptr);
		/* Strip byte count flag, long int */
		BM_WriteHexString(fptr,"0117000400000001");
		offset = nx * ny * 3;
		BM_WriteLongInt(fptr,buffer,offset);
		/* Minimum sample value flag, short int */
		BM_WriteHexString(fptr,"0118000300000003");
		offset = nx * ny * 3 + 188;
		BM_WriteLongInt(fptr,buffer,offset);
		/* Maximum sample value tag, short int */
		BM_WriteHexString(fptr,"0119000300000003");
		offset = nx * ny * 3 + 194;
		BM_WriteLongInt(fptr,buffer,offset);
		/* Planar configuration tag, short int */
		BM_WriteHexString(fptr,"011c00030000000100010000");
		/* Sample format tag, short int */
		BM_WriteHexString(fptr,"0153000300000003");
		offset = nx * ny * 3 + 200;
		BM_WriteLongInt(fptr,buffer,offset);
		/* End of the directory entry */
		BM_WriteHexString(fptr,"00000000");
		/* Bits for each colour channel */
		BM_WriteHexString(fptr,"000800080008");
		/* Minimum value for each component */
		BM_WriteHexString(fptr,"000000000000");
		/* Maximum value per channel */
		BM_WriteHexString(fptr,"00ff00ff00ff");
		/* Samples per pixel for each channel */
		BM_WriteHexString(fptr,"000100010001");
		break;
//---------------------------------------------- EPS format (color)
//---------------------------------------------- EPS format (black and white)
	case 1:
		fprintf(fptr,"\n%%%%EOF\n");
		break;
	}

    fclose(fptr);
}




GLOBAL(void) write_JPEG_file (char *fName, FILE *fraw, int width, int height)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

    long bitsize;
    unsigned char *image_buffer;

	FILE * outfile;
	JSAMPROW row_pointer[1];
	int row_stride;
	int j;
	long offset;
	int quality;
	int smoothing;
	int scale;

	quality = 100;
	smoothing = 1;
	scale = 1;

// Step 1: allocate and initialize JPEG compression object
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	
// Step 2: specify data destination (eg, a file)
	outfile = fopen(fName, "wb");
	jpeg_stdio_dest(&cinfo, outfile);

// Step 3: set parameters for compression
	cinfo.image_width = width * scale;
	cinfo.image_height = height * scale;
	cinfo.input_components = 3;		
	cinfo.in_color_space = JCS_RGB;	
	jpeg_set_defaults(&cinfo);
	cinfo.smoothing_factor = smoothing;
	cinfo.write_Adobe_marker = TRUE;
	jpeg_set_quality(&cinfo, quality, TRUE);

// Step 4: Start compressor
	jpeg_start_compress(&cinfo, TRUE);

// Step 5: while (scan lines remain to be written) jpeg_write_scanlines(...);
	bitsize = 3 * width * scale;
	if ((image_buffer = malloc(bitsize)) == NULL) {
		fprintf(stderr,"Failed to allocate memory for image\n");
	}
	free(image_buffer);
        row_stride = cinfo.image_width * 3;
	for (j=0; j<height*scale; j++){
		image_buffer = malloc(bitsize);
		offset = (height*scale-j-1)*bitsize;
		fseek( fraw, offset, SEEK_SET);
		fread(image_buffer,1,bitsize,fraw);
		row_pointer[0] =  & image_buffer[0];
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
		free(image_buffer);
	}

// Step 6: Finish compression close the output file
	jpeg_finish_compress(&cinfo);
	fclose(outfile);

// Step 7: release JPEG compression object
	jpeg_destroy_compress(&cinfo);
}



//-------------------------------------------------------------//
//	Write a bitmap to a temp. BMP file                         //
//-------------------------------------------------------------//
void CreateBMPtemp(FILE *fptr, FILE *fraw, int nx, int ny)
{
	int size;
	unsigned char *bm;
	long bitsize,i,j,index,width;

	width   = nx * 3;     /* Real width of scanline */
	width   = (width + 3) & ~3;    /* Aligned to 4 bytes */
	bitsize = width * ny; /* Size of bitmap, aligned */

		/* Header 10 bytes */
	write_word(fptr, BF_TYPE);
	size = bitsize + 14 + 40;
    write_dword(fptr, size);
	write_word(fptr, 0);
	write_word(fptr, 0);
		/* Offset to image data */
	write_dword(fptr, 14+40);
		/* Information header 40 bytes */
	write_dword(fptr, 40);
	write_long(fptr, nx);
	write_long(fptr, ny);
	write_word(fptr, 1);
	write_word(fptr, 24);
		/* Compression type == 0 */
	write_dword(fptr, 0);
	write_dword(fptr, bitsize);
	write_long(fptr, 2952);
	write_long(fptr, 2952);
	write_dword(fptr, 0);
	write_dword(fptr, 0);

	putc(1,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr); 
	putc(1,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr); 
	putc(0,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr); /* No palette */
	putc(0,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr); 
	
	/* Write the binary data */
	bitsize = 3 * nx;
	for (j=0; j<ny; j++){
		bm = malloc(bitsize);
		fread(bm,1,bitsize,fraw);
		for(i=0; i<nx; i++){
			index =3*i;
			putc(bm[index+1],fptr);
			putc(bm[index+0],fptr);
			putc(bm[index+2],fptr);
		}
		free(bm);
	}
}

//***************************************************************************//
int write_JPEG2000_file(char *fName, FILE *fraw, int width, int height)
{
    FILE *bmpfptr;
	FILE *outfp;
	char *temp_dir;

	jas_image_t *image;
	cmdopts_t *cmdopts;
	jas_stream_t *in;
	jas_stream_t *out;

	temp_dir = "";
	bmpfptr = tmpfile(); 
	printf("Create temp. BMP image file\n");

	CreateBMPtemp(bmpfptr, fraw, width, height);
	rewind(bmpfptr);

	printf("Convert image file from BMP to JPEG2000 format. Working...\n");
	outfp = fopen(fName,"w+b");
	jas_init();
	if (!(cmdopts = malloc(sizeof(cmdopts_t)))) {
		fprintf(stderr, "error: insufficient memory\n");
		return EXIT_FAILURE;
	}
	cmdopts->infmt = 2;
	cmdopts->inopts = 0;
	cmdopts->inoptsbuf[0] = '\0';
	cmdopts->outfmt = 4;
	cmdopts->outopts = 0;
	cmdopts->outoptsbuf[0] = '\0';

	in = jas_stream_freopen(NULL, "rb", bmpfptr);
	out = jas_stream_freopen(NULL, "w+b", outfp);

	if (!(image = jas_image_decode(in, cmdopts->infmt, cmdopts->inopts))) {
		fprintf(stderr, "error: cannot load image data\n");
		return EXIT_FAILURE;
	}
	if (jas_image_encode(image, out, cmdopts->outfmt, cmdopts->outopts)) {
		fprintf(stderr, "error: cannot encode image\n");
		return EXIT_FAILURE;
	}
	jas_stream_flush(out);
	(void) jas_stream_close(in);
	if (jas_stream_close(out)) {
		fprintf(stderr, "error: cannot close output image file\n");
		return EXIT_FAILURE;
	}
	free(cmdopts);
	jas_image_destroy(image);
	jas_image_clearfmts();

	fclose(bmpfptr);
	printf("...................OK!!!\n");

	/* Success at last! :-) */
	return EXIT_SUCCESS;
}




