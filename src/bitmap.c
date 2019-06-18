/**
 * @file bitmap.c
 * @author Philip R. Simonson
 * @date 16 June 2019
 * @brief Bitmap file loader/creator.
 *
 * This loader also has some special functionality.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "bitmap.h"

int _bitmap_errno; /**< Current error code from bitmap library. */

/**
 * @brief Create a new bitmap
 *
 * Prototype:
 *   Bitmap *create_bitmap(int width, int height)
 */
Bitmap*
create_bitmap (int width, int height)
{
	Bitmap* bitmap = malloc(sizeof(Bitmap));
	if (!bitmap) {
		_bitmap_errno = BMP_MALLOC_ERROR;
		return NULL;
	}

	memset(&bitmap->info, 0, sizeof(BitmapInfo));
	/* init file header */
	bitmap->info.type = 0x4D42;
	bitmap->info.fsize = sizeof(BitmapInfo)+(width*height*3);
	bitmap->info.res1 = 0;
	bitmap->info.res2 = 0;
	bitmap->info.offset = sizeof(BitmapInfo);
	/* init bitmap header */
	bitmap->info.size = 40;
	bitmap->info.width = width;
	bitmap->info.height = height;
	bitmap->info.planes = 1;
	bitmap->info.bpp = 24;
	bitmap->info.compression = 0;
	bitmap->info.isize = width*height*3;
	bitmap->info.hres = 0;
	bitmap->info.vres = 0;
	bitmap->info.palette = 0;
	bitmap->info.impcolors = 0;

	bitmap->data = malloc(bitmap->info.isize);
	if (!bitmap->data) {
		_bitmap_errno = BMP_MALLOC_ERROR;
		destroy_bitmap(bitmap);
		return NULL;
	}
	memset(bitmap->data, 0x00, bitmap->info.isize);
	return bitmap;
}

/**
 * @brief Gets the data from a bitmap object.
 *
 * Prototype:
 *   Bitmap *load_bitmap(const char *filename)
 */
Bitmap*
load_bitmap (const char *filename)
{
	Bitmap *bmp;
	file_t file;

	init_file(&file);
	open_file(&file, filename, "rb");
	if (get_errori_file(&file) != FILE_ERROR_OKAY) {
		_bitmap_errno = BMP_FILE_ERROR;
		return NULL;
	}

	bmp = malloc(sizeof(Bitmap));
	if (!bmp) {
		_bitmap_errno = BMP_MALLOC_ERROR;
		close_file(&file);
		return NULL;
	}

	/* get bitmap info */
	read_file(&file, &bmp->info.type, 2, 1);
	read_file(&file, &bmp->info.fsize, 4, 1);
	read_file(&file, &bmp->info.res1, 2, 1);
	read_file(&file, &bmp->info.res2, 2, 1);
	read_file(&file, &bmp->info.offset, 4, 1);
	read_file(&file, &bmp->info.size, 4, 1);
	read_file(&file, &bmp->info.width, 4, 1);
	read_file(&file, &bmp->info.height, 4, 1);
	read_file(&file, &bmp->info.planes, 2, 1);
	read_file(&file, &bmp->info.bpp, 2, 1);
	read_file(&file, &bmp->info.compression, 4, 1);
	read_file(&file, &bmp->info.isize, 4, 1);
	read_file(&file, &bmp->info.hres, 4, 1);
	read_file(&file, &bmp->info.vres, 4, 1);
	read_file(&file, &bmp->info.palette, 4, 1);
	read_file(&file, &bmp->info.impcolors, 4, 1);

	if (bmp->info.type != 0x4D42 && bmp->info.fsize != bmp->info.isize) {
		_bitmap_errno = BMP_TYPE_ERROR;
		close_file(&file);
		return NULL;
	}

	/* load bitmap data */
	bmp->data = malloc(bmp->info.isize);
	if (!bmp->data) {
		_bitmap_errno = BMP_MALLOC_ERROR;
		free(bmp);
		close_file(&file);
		return NULL;
	}
	memset(bmp->data, 0, bmp->info.isize);
	seek_file(&file, bmp->info.offset, SEEK_SET);
	read_file(&file, bmp->data, 1, bmp->info.isize);
	close_file(&file);
	return bmp;
}

/**
 * @brief Writes blank Bitmap image if not data has been given.
 *
 * Prototype:
 *   int write_bitmap(Bitmap *bmp, const char *filename)
 */
int
write_bitmap (Bitmap *bmp, const char *filename)
{
	file_t file;
	int res;

	init_file(&file);
	open_file(&file, filename, "wb");
	if (get_errori_file(&file) != FILE_ERROR_OKAY) {
		_bitmap_errno = BMP_FILE_ERROR;
		return 1;
	}

	res = write_file(&file, &bmp->info, 1, sizeof(BitmapInfo));
	if (res < (int)sizeof(BitmapInfo)) {
		_bitmap_errno = BMP_FILE_ERROR;
		close_file(&file);
		return 1;
	}
	res = write_file(&file, bmp->data, 1, bmp->info.isize);
	if (res < bmp->info.isize) {
		_bitmap_errno = BMP_FILE_ERROR;
		close_file(&file);
		return 1;
	}
	close_file(&file);
	if (get_errori_file(&file) != FILE_ERROR_CLOSED) {
		_bitmap_errno = BMP_FILE_ERROR;
		return 1;
	}
	return 0;
}

/**
 * @brief Checks for valid pixels before plotting.
 *
 * Function used internally doesn't export.
 */
static int
_check_pixel_bitmap (Bitmap *bmp, int y, int x)
{
	if (x < 0 || x > bmp->info.width)
		return 1;
	if (y < 0 || y > bmp->info.height)
		return 1;
	return 0;
}

/**
 * @brief Get a pixel at (x,y) coordinates r, g, b values.
 *
 * Prototype:
 *   void get_pixel_bitmap(Bitmap *bmp, int y, int x, Color *pixel)
 */
void
get_pixel_bitmap (Bitmap *bmp, int y, int x, Color *pixel)
{
	if (_check_pixel_bitmap(bmp, y, x)) {
		_bitmap_errno = BMP_PIXEL_ERROR;
		return;
	}
	pixel->b = bmp->data[(bmp->info.width*y+x)*3];
	pixel->g = bmp->data[(bmp->info.width*y+x)*3+1];
	pixel->r = bmp->data[(bmp->info.width*y+x)*3+2];
}

/**
 * @brief Put a pixel at (x,y) coordinates r, g, b values.
 *
 * Prototype:
 *   void set_pixel_bitmap(Bitmap *bmp, int y, int x, Color pixel)
 */
void
set_pixel_bitmap (Bitmap *bmp, int y, int x, Color pixel)
{
	if (_check_pixel_bitmap(bmp, y, x)) {
		_bitmap_errno = BMP_PIXEL_ERROR;
		return;
	}
	bmp->data[(bmp->info.width*y+x)*3] = pixel.b;
	bmp->data[(bmp->info.width*y+x)*3+1] = pixel.g;
	bmp->data[(bmp->info.width*y+x)*3+2] = pixel.r;
}

/**
 * @brief Fill an entire bitmap with a color.
 *
 * Prototype:
 *   void fill_bitmap(Bitmap *bmp, Color pixel)
 */
void
fill_bitmap (Bitmap *bmp, Color pixel)
{
	int x,y;

	for(y=0; y<bmp->info.height; y++)
		for(x=0; x<bmp->info.width; x++)
			set_pixel_bitmap(bmp, y, x, pixel);
}

/**
 * @brief Draws a line horizontal or vertical.
 */
void
draw_line_bitmap (Bitmap *bmp, int start, char flipped,
		char vertical, int len, Color col)
{
	int i;

	if (len >= bmp->info.width || len >= bmp->info.height)
		return;
	if (flipped) {
		for(i=start+len; i>=start; i--)
			if (vertical)
				set_pixel_bitmap(bmp, i, start+len, col);
			else
				set_pixel_bitmap(bmp, start+len, i, col);
	} else {
		for(i=start; i<=start+len; i++)
			if (vertical)
				set_pixel_bitmap(bmp, i, start, col);
			else
				set_pixel_bitmap(bmp, start, i, col);
	}
}



/**
 * @brief Draws a circle from start to given radius.
 */
void
draw_circle_bitmap (Bitmap *bmp, int x_centre, int y_centre, int r, Color pixel)
{
	int x,y;
	int P;

	x = r;
	y = 0;

	printf("(%d, %d) ", y+y_centre, x+x_centre);
	set_pixel_bitmap(bmp, y+y_centre, x+x_centre, pixel);

	if (r > 0) {
		printf("(%d, %d) ", -y+y_centre, x+x_centre);
		printf("(%d, %d) ", x+y_centre, y+x_centre);
		printf("(%d, %d)\n", x+y_centre, -y+x_centre);

		set_pixel_bitmap(bmp, -y+y_centre, x+x_centre, pixel);
		set_pixel_bitmap(bmp, x+y_centre, y+x_centre, pixel);
		set_pixel_bitmap(bmp, x+y_centre, -y+x_centre, pixel);
	}

	P = 1 - r;
	while (x > y) {
		y++;
		if (P <= 0)
			P = P + 2*y + 1;
		else {
			x--;
			P = P + 2*y - 2*x + 1;
		}

		if (x < y)
			break;

		printf("(%d, %d) ", y+y_centre, x+x_centre);
		printf("(%d, %d) ", y+y_centre, -x+x_centre);
		printf("(%d, %d) ", -y+y_centre, x+x_centre);
		printf("(%d, %d)\n", -y+y_centre, -x+x_centre);

		set_pixel_bitmap(bmp, y+y_centre, x+x_centre, pixel);
		set_pixel_bitmap(bmp, y+y_centre, -x+x_centre, pixel);
		set_pixel_bitmap(bmp, -y+y_centre, x+x_centre, pixel);
		set_pixel_bitmap(bmp, -y+y_centre, -x+x_centre, pixel);

		if (x != y) {
			printf("(%d, %d) ", y+x_centre, x+y_centre);
			printf("(%d, %d) ", -y+x_centre, x+y_centre);
			printf("(%d, %d) ", y+x_centre, -x+y_centre);
			printf("(%d, %d)\n", -y+x_centre, -x+y_centre);

			set_pixel_bitmap(bmp, x+y_centre, y+x_centre, pixel);
			set_pixel_bitmap(bmp, x+y_centre, -y+x_centre, pixel);
			set_pixel_bitmap(bmp, -x+y_centre, y+x_centre, pixel);
			set_pixel_bitmap(bmp, -x+y_centre, -y+x_centre, pixel);
		}
	}
}

/**
 * @brief Draws a set amount of squares diagonally.
 */
void
draw_squares_bitmap (Bitmap *bmp, int start, int count,
		Color pixel)
{
	int i;
	for (i=1; i<=count; i++) {
		/* draw lines */
		draw_line_bitmap(bmp, start*i, 0, 1, start, pixel);
		draw_line_bitmap(bmp, start*i, 0, 0, start, pixel);
		draw_line_bitmap(bmp, start*i, 1, 1, start, pixel);
		draw_line_bitmap(bmp, start*i, 1, 0, start, pixel);
	}
}

/**
 * @brief Flip image upside down.
 */
void
flip_vertical_bitmap (Bitmap **bitmap)
{
	Bitmap *bmp;
	int x,y;

	bmp = malloc(sizeof(Bitmap));
	if (!bmp) {
		_bitmap_errno = BMP_MALLOC_ERROR;
		destroy_bitmap(*bitmap);
		return;
	}
	bmp->data = malloc((*bitmap)->info.isize);
	if (!bmp->data) {
		_bitmap_errno = BMP_MALLOC_ERROR;
		destroy_bitmap(*bitmap);
		free(bmp);
		return;
	}
	bmp->info = (*bitmap)->info;
	memset(bmp->data, 0, bmp->info.isize);

	/* finally redo image data */
	for(y=0; y<bmp->info.height; y++) {
		for(x=0; x<bmp->info.width; x++) {
			Color tmp;
			get_pixel_bitmap(*bitmap, y, x, &tmp);
			set_pixel_bitmap(bmp, (bmp->info.height-1)-y, x, tmp);
		}
	}
	destroy_bitmap(*bitmap);
	*bitmap = bmp;
}

/**
 * @brief Sets all pixels random in bitmap image.
 */
void
randomise_bitmap (Bitmap *bmp)
{
	int i,j;

	if (!bmp) {
		printf("Error: Bitmap object doesn't exist.\n");
		return;
	}

	for(i=0; i<bmp->info.height; i++)
		for(j=0; j<bmp->info.width; j++) {
			Color pixel = {rand()%255,rand()%255,rand()%255};
			set_pixel_bitmap(bmp, i, j, pixel);
		}
}

/**
 * @brief Convert a bitmap to greyscale.
 */
void
bitmap_to_greyscale (Bitmap *bmp)
{
	int x,y;

	for(y=0; y<bmp->info.height; y++)
		for(x=0; x<bmp->info.width; x++) {
			Color pixel;
			int color;

			get_pixel_bitmap(bmp, y, x, &pixel);
			color = (int)((pixel.r*0.21)+(pixel.g*0.72)+(pixel.b*0.07));
			pixel.r = color;
			pixel.g = color;
			pixel.b = color;
			set_pixel_bitmap(bmp, y, x, pixel);
		}
}

/**
 * @brief Embeds text into a bitmap.
 */
void
encode_steganograph (Bitmap *bmp, const char *msg)
{
	static long offset = 0;
	unsigned char byte = 0xf0;
	int i,j;

	if (!bmp || !msg)
		return;

	if ((int)(strlen(msg)+40) > bmp->info.isize) {
		printf("Error: image size not big enough.\n");
		return;
	}
	for(j=7; j>=0; --j,++offset) {
		char add = (byte >> j) & 1;
		bmp->data[offset] = (bmp->data[offset] & 0xFE) | add;
	}
	for(j=31; j>=0; --j,++offset) {
		char add = (strlen(msg) >> j) & 1;
		bmp->data[offset] = (bmp->data[offset] & 0xFE) | add;
	}
	for(i=0; i<=(int)strlen(msg); ++i) {
		for(j=7; j>=0; --j,++offset) {
			int b = (msg[i] >> j) & 1;
			bmp->data[offset] = ((bmp->data[offset] & 0xFE) | b);
		}
	}
}

/**
 * @brief Gets the embedded text from a bitmap.
 */
char*
decode_steganograph(Bitmap *bmp)
{
	char *buf;
	int i, j, length = 0;
	static long offset = 40;
	unsigned char byte;

	if (!bmp)
		return NULL;

	byte = 0;
	for(i=0; i<8; ++i)
		byte = (byte << 1) | (bmp->data[i] & 1);

	if (byte != 0xf0) {
		_bitmap_errno = BMP_DECODE_ERROR;
		destroy_bitmap(bmp);
		return NULL;
	}

	for(i=8; i<40; ++i)
		length = (length << 1) | (bmp->data[i] & 1);

	buf = malloc(length+1);
	if (!buf) {
		_bitmap_errno = BMP_MALLOC_ERROR;
		return NULL;
	}
	memset(buf, 0, length+1);
	for(i=0; i<length; i++) {
		for(j=0; j<8; ++j,++offset)
			buf[i] = (buf[i] << 1) | (bmp->data[offset] & 1);
	}
	buf[length+1] = '\0';
	return buf;
}

/**
 * @brief Free up all memory for bitmap.
 */
void
destroy_bitmap (Bitmap *bitmap)
{
	free(bitmap->data);
	free(bitmap);
}

/**
 * @brief Gets last error code from my library.
 */
int
get_last_error_bitmap ()
{
	int bitmap_errno = _bitmap_errno;
	_bitmap_errno = 0;
	switch(bitmap_errno) {
		case BMP_NO_ERROR:
#ifdef DEBUG
			printf("[DEBUG] <BITMAP>: No Error!\n");
#endif
			break;
		case BMP_MALLOC_ERROR:
			printf("Error: Out of memory!\n");
			break;
		case BMP_FILE_ERROR:
			fprintf(stderr,
				"Error: File operation not successful.\n");
			break;
		case BMP_PIXEL_ERROR:
			fprintf(stderr,
				"Error: Cannot get/set pixel data.\n");
			break;
		case BMP_TYPE_ERROR:
			fprintf(stderr,
				"Error: File isn't a bitmap image.\n");
			break;
		case BMP_DECODE_ERROR:
			fprintf(stderr,
				"Error: File doesn't contain hidden text.\n");
			break;
		default:
#ifdef DEBUG
			fprintf(stderr,
				"[DEBUG] <BITMAP>: Unknown Error.\n");
#endif
			break;
	}
	return bitmap_errno;
}
