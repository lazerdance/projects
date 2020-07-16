/*
Author: Cornelius Marx
Date: April 10th 2019
Description:
	Small bmp loader/saver. Stores data internally as RGBA.
	Can load 24bit images. No color indexing supported.
*/
#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef unsigned int Color;

/* image storing bmp file */
typedef struct{
	int w; /* image width */
	int h; /* image height */	
	Color * pixels; /* pixels from top left to bottom right */
}bmp_Image;

/* bmp_Rect describes an axis-aligned rectangle with its origin at the upper left corner */
typedef struct{
	int x; /* x-position of the upper left corner */
	int y; /* y-position of the upper left corner */
	int w; /* width of the rect */
	int h; /* height of the rect */
}bmp_Rect;

/* magic number at the beginning of the header (bfType) */
#define BMP_MAGIC_NUMBER 0x4D42

/* bmp header */
#define BMP_HEADER_SIZE 14 
typedef struct __attribute__((__packed__)){
	uint16_t bfType;
	uint32_t bfSize;
	uint32_t bfReserved;
	uint32_t bfOffBits;
}bmp_FileHeader;

/* print given info to stdout */
void bmp_printHeader(const bmp_FileHeader * header);

/* bmp info */
#define BMP_INFO_SIZE 40 
typedef struct __attribute__((__packed__)){
	uint32_t biSize;
	int32_t biWidth;
	int32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t biXPelsPerMeter;
	int32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
}bmp_Info;

/* get color from integer r,g,b (=[0, 255]) component */
Color bmp_getRGBColor(unsigned char r, unsigned char g, unsigned char b);

/* get color from floating r,g,b (=[0.0, 1.0]) component */
Color bmp_getRGBColorf(float r, float g, float b);

/* print given info to stdout */
void bmp_printInfo(const bmp_Info * info);

/* load image from file
	filename: bmp file to load
	return: the image loaded from file or NULL on error
*/
bmp_Image* bmp_loadFromFile(const char * filename);

/* load image from data
	w: width of the image (> 0)
	h: height of the image (> 0)
	data: buffer containing w*h Color pixels, NULL can be passed to fill entire image with 0s (black)
		data will be copied to a new buffer, so the given buffer can be safely freed after this call
	return: the image loaded from data
*/
bmp_Image* bmp_loadFromData(int w, int h, const Color * data);

/* getting index in pixel buffer according to the pixel with the coordinates (x,y)
	x,y: coordinates of pixel (origin at top left corner)
	img: image to get index from
	return: index in color buffer, or -1 if coordinates are out of range
*/
int bmp_getIndex(int x, int y, const bmp_Image * img);

/* copy pixel data of one image (src_img) to another image (dst_img)
	src_img: image to copy pixels from
	dst_img: destination image
	dst_x, dst_y: origin position of src_img will be placed at that position with respect to
	the origin of the dst_img. NOTE: position can be set negative
*/
void bmp_stamp(const bmp_Image * src_img, bmp_Image * dst_img, int dst_x, int dst_y);

/* save image to file 
	img: image to store
	filname: filename of the bmp file to save
	return: number of bytes written to file (header + image data), 0 on error
*/
size_t bmp_saveToFile(const bmp_Image * img, const char * filename);

/* free image data
	img: the image to free
*/
void bmp_free(bmp_Image * img);

#endif
