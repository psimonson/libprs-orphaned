/**
 * @file bitmap.h
 * @author Philip R. Simonson
 * @date   2019/05/18
 * @brief Simple bitmap file format handler.
 ********************************************************************
 * @details
 *
 * Simple bitmap file format handler. Create a blank bitmap image,
 * Load a bitmap image, Convert to greyscale, Write image to new
 * file, Encode/Decode steganography (put text inside image), etc.
 ********************************************************************
 */

#ifndef PRS_BITMAP_H
#define PRS_BITMAP_H

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Bitmap library errors enumeration. */
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

/** @brief BITMAPINFO structure. */
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

/** @brief BITMAP structure (externally used). */
typedef struct BITMAP {
	BitmapInfo info;
	unsigned char *data;
} Bitmap;

/** @brief Color structure (externally used). */
typedef struct Color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} Color;

/** @brief Get a pixel from the bitmap. */
PRS_EXPORT void get_pixel_bitmap(Bitmap *bitmap, int y, int x, Color *pixel);
/** @brief Set a pixel in the bitmap. */
PRS_EXPORT void set_pixel_bitmap(Bitmap *bitmap, int y, int x, Color pixel);
/** @brief Fills bitmap with color. */
PRS_EXPORT void fill_bitmap(Bitmap *bitmap, Color pixel);

/** @brief Draw a line inside the bitmap. */
PRS_EXPORT void draw_line_bitmap(Bitmap *bitmap, int start, char flipped,
		char vertical, int len, Color pixel);
/** @brief Draw a circle inside the bitmap. */
PRS_EXPORT void draw_circle_bitmap(Bitmap *bitmap, int w, int h, int r, Color pixel);
/** @brief Draw squares inside of the bitmap. */
PRS_EXPORT void draw_squares_bitmap(Bitmap *bitmap, int start, int count, Color pixel);

/** @brief Create a blank bitmap file. */
PRS_EXPORT Bitmap *create_bitmap(int width, int height);
/** @brief Load an existing bitmap file. */
PRS_EXPORT Bitmap *load_bitmap(const char *filename);
/** @brief Write/Overwrite a bitmap file. */
PRS_EXPORT int write_bitmap(Bitmap *bitmap, const char *filename);
/** @brief Flip bitmap vertically. */
PRS_EXPORT void flip_vertical_bitmap(Bitmap **bitmap);
/** @brief Randomise data inside bitmap. */
PRS_EXPORT void randomise_bitmap(Bitmap *bitmap);
/** @brief Convert bitmap to greyscale. */
PRS_EXPORT void bitmap_to_greyscale(Bitmap *bitmap);
/** @brief Encode steganography, hide text inside bitmap. */
PRS_EXPORT void encode_steganograph(Bitmap *bitmap, const char *msg);
/** @brief Decode steganogrpahy, show hidden text inside bitmap. */
PRS_EXPORT char* decode_steganograph(Bitmap *bitmap);
/** @brief Free bitmap structure memory. */
PRS_EXPORT void destroy_bitmap(Bitmap *bitmap);
/** @brief Gets last error code from bitmap structure. */
PRS_EXPORT int get_last_error_bitmap();

#ifdef __cplusplus
}
#endif

#endif
