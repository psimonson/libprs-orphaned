/*
 * bitmap.h - simple bitmap file handling header file.
 *
 * Author: Philip R. Simonson
 * Date  : 2019/05/18
 ********************************************************************
 */

#define PRS_BITMAP_H

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Bitmap library errors enumeration */
typedef enum {
	BMP_NO_ERROR,
	BMP_MALLOC_ERROR,
	BMP_FILE_ERROR,
	BMP_PIXEL_ERROR,
	BMP_TYPE_ERROR,
	BMP_DECODE_ERROR
} BitmapError;

#ifndef BYTE
typedef char BYTE;
typedef short WORD;
typedef int DWORD;
typedef long QWORD;
#endif

#pragma pack(push, 1)
typedef struct BITMAPINFO {
	/* info on file */
	WORD  type;
	DWORD fsize;
	WORD  res1;
	WORD  res2;
	DWORD offset;
	/* info on image */
	DWORD size;
	DWORD width;
	DWORD height;
	WORD  planes;
	WORD  bpp;
	DWORD compression;
	DWORD isize;
	DWORD hres;
	DWORD vres;
	DWORD palette;
	DWORD impcolors;
} BitmapInfo;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BITMAP {
	BitmapInfo info;
	unsigned char *data;
} Bitmap;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct Color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} Color;
#pragma pack(pop)

/* pixel functions */
PRS_EXPORT void get_pixel_bitmap(Bitmap *bitmap, int y, int x, Color *pixel);
PRS_EXPORT void set_pixel_bitmap(Bitmap *bitmap, int y, int x, Color pixel);
PRS_EXPORT void fill_bitmap(Bitmap *bitmap, Color pixel);

/* drawing functions */
PRS_EXPORT void draw_line_bitmap(Bitmap *bitmap, int start, char flipped,
		char vertical, int len, Color pixel);
PRS_EXPORT void draw_circle_bitmap(Bitmap *bitmap, int w, int h, int r, Color pixel);
PRS_EXPORT void draw_squares_bitmap(Bitmap *bitmap, int start, int count, Color pixel);

/* main bitmap functions */
PRS_EXPORT Bitmap *create_bitmap(int width, int height);
PRS_EXPORT Bitmap *load_bitmap(const char *filename);
PRS_EXPORT int write_bitmap(Bitmap *bitmap, const char *filename);
PRS_EXPORT void flip_vertical_bitmap(Bitmap **bitmap);
PRS_EXPORT void randomise_bitmap(Bitmap *bitmap);
PRS_EXPORT void bitmap_to_greyscale(Bitmap *bitmap);
PRS_EXPORT void encode_steganograph(Bitmap *bitmap, const char *msg);
PRS_EXPORT char* decode_steganograph(Bitmap *bitmap);
PRS_EXPORT void destroy_bitmap(Bitmap *bitmap);
PRS_EXPORT int get_last_error_bitmap();

#ifdef __cplusplus
}
#endif
