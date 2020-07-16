/*
Author: Cornelius Marx
Date: April 10th 2019
*/
#include "bmp.h"

Color bmp_getRGBColor(unsigned char r, unsigned char g, unsigned char b)
{
	return 0xFF000000 | (r<<16) | (g<<8) | b;
}

Color bmp_getRGBColorf(float r, float g, float b)
{
	/* clamp color to [0, 1] */
	if(r < 0.f)
		r = 0.f;
	else if(r > 1.f)
		r = 1.f;

	if(g < 0.f)
		g = 0.f;
	else if(g > 1.f)
		g = 1.f;

	if(b < 0.f)
		b = 0.f;
	else if(b > 1.f)
		b = 1.f;

	return bmp_getRGBColor((unsigned char)(255*r), (unsigned char)(255*g), (unsigned char)(255*b));
}

void bmp_printHeader(const bmp_FileHeader * header)
{
	printf("Type: '%c%c'\n", ((char*)&header->bfType)[0], ((char*)&header->bfType)[1]);
	printf("Size: %d\n", header->bfSize);
	printf("Reserved: %d\n", header->bfReserved);
	printf("OffBits: %d\n", header->bfOffBits);
}

void bmp_printInfo(const bmp_Info * info)
{
	printf("Size: %d\n", info->biSize);
	printf("Width: %d\n", info->biWidth);
	printf("Height: %d\n", info->biHeight);
	printf("Planes: %d\n", info->biPlanes);
	printf("BitCount: %d\n", info->biBitCount);
	printf("Compression: %d\n", info->biCompression);
	printf("SizeImage: %d\n", info->biSizeImage);
	printf("XPelsPerMeter: %d\n", info->biXPelsPerMeter);
	printf("YPerPelsMeter: %d\n", info->biYPelsPerMeter);
	printf("ClrUsed: %d\n", info->biClrUsed);
	printf("ClrImportant: %d\n", info->biClrImportant);
}

bmp_Image* bmp_loadFromData(int w, int h, const Color * data)
{
	if(w < 0 || h < 0)/* invalid dimension */
		return NULL; 

	/* creating new pixel buffer and copy over */
	size_t pixel_size = w*h;
	Color * pixels = (Color*)malloc(pixel_size*sizeof(Color));
	if(data == NULL)
	{
		memset(pixels, 0, pixel_size*sizeof(Color));
	}
	else
	{
		memcpy(pixels, data, pixel_size*sizeof(Color));
	}

	/* creating image */
	bmp_Image * img = (bmp_Image*)malloc(sizeof(bmp_Image));
	img->w = w;
	img->h = h;
	img->pixels = pixels;
	
	return img;
}

bmp_Image* bmp_loadFromFile(const char * filename)
{
	/* open file for binary reading */
	FILE * f = fopen(filename, "rb");

	if(f == NULL)/* unable to open file */
	{
		printf("Unable to open file '%s'", filename);
		return NULL;
	}

	/* reading header and info */
	bmp_FileHeader header;
	bmp_Info info;
	if(fread(&header, 1, BMP_HEADER_SIZE, f) != BMP_HEADER_SIZE)
	{
		fclose(f);
		return NULL;
	}
	if(header.bfType != BMP_MAGIC_NUMBER)
	{
		fclose(f);
		return NULL;
	}
	if(fread(&info, 1, BMP_INFO_SIZE, f) != BMP_INFO_SIZE)
	{
		fclose(f);
		return NULL;
	}

	/* seeking to start of image data */
	fseek(f, info.biSize+BMP_HEADER_SIZE, SEEK_SET);

	/* checking image size */
	if(info.biWidth <= 0)
	{
		fclose(f);
		return NULL;
	}
	int real_h = info.biHeight;
	if(real_h < 0)
		real_h = -real_h;
	size_t num_pixels = real_h*info.biWidth;	
	uint8_t * pixels32 = (uint8_t*)malloc(num_pixels*sizeof(Color));
	/* reading image data */
	if(info.biBitCount == 24)
	{
		/* transforming from 24Bit to 32Bit */
		size_t row_size = 4 * ((24*info.biWidth+31)/32);
		size_t img_size = row_size*real_h;
		uint8_t * pixels24 = (uint8_t*)malloc(img_size);
		if(fread(pixels24, 1, img_size, f) != img_size)
		{
			free(pixels24);
			free(pixels32);
			fclose(f);
			return NULL;
		}
		for(int y = real_h-1; y >= 0; y--)
		{
			size_t base32 = (real_h-1-y)*info.biWidth*4;
			size_t base24 = y*row_size;
			for(int x = 0; x < info.biWidth; x++)
			{
				size_t index32 = base32 + x*4;
				size_t index24 = base24 + x*3;
				pixels32[index32+0] = pixels24[index24+0];
				pixels32[index32+1] = pixels24[index24+1];
				pixels32[index32+2] = pixels24[index24+2];
				pixels32[index32+3] = 255;
			}
		}
		free(pixels24);
	}
	else{/* only 24Bit supported */
		free(pixels32);
		fclose(f);
		return NULL;
	}
	/* done reading from file */
	fclose(f);

	/* creating image */
	bmp_Image * img = (bmp_Image*)malloc(sizeof(bmp_Image));
	img->w = info.biWidth;
	img->h = real_h;
	img->pixels = (Color*)pixels32;
	return img;
}

int bmp_getIndex(int x, int y, const bmp_Image * img)
{
	if(x < 0 || x >= img->w ||
		y < 0 || y >= img->h)/* boundary check */
		return -1;
	
	return y*img->w + x;
}

void bmp_stamp(const bmp_Image * src_img, bmp_Image * dst_img, int dst_x, int dst_y)
{
	/* getting start and end position in dst_img */
	int start_x;
	int start_y;
	if(dst_x < 0)
		start_x = 0; 
	else
		start_x = dst_x;

	if(dst_y < 0)
		start_y = 0;
	else
		start_y = dst_y;
	
	int end_x;
	int end_y;
	if(dst_x+src_img->w > dst_img->w)
		end_x = dst_img->w;
	else
		end_x = dst_x+src_img->w;

	if(dst_y+src_img->h > dst_img->h)
		end_y = dst_img->h;
	else
		end_y = dst_y+src_img->h;
	
	for(int x = start_x; x < end_x; x++)
	{
		for(int y = start_y; y < end_y; y++)
		{
			int dst_i = bmp_getIndex(x, y, dst_img);
			int src_i = bmp_getIndex(x-dst_x, y-dst_y, src_img);
			dst_img->pixels[dst_i] = src_img->pixels[src_i];
		}
	}
}

size_t bmp_saveToFile(const bmp_Image * img, const char * filename)
{
	/* open file for binary writing */
	FILE * f = fopen(filename, "wb");
	if(f == NULL)
	{
		return 0;
	}
	size_t bytes_written = 0;
	/* writing header and info */
	size_t row_size = 4 * ((24*img->w+31)/32);
	size_t img_size = img->h * row_size;
	int padding = row_size - img->w*3;
	bmp_FileHeader header;
	memset(&header, 0, sizeof(header));
	header.bfType = BMP_MAGIC_NUMBER;
	header.bfSize = BMP_HEADER_SIZE + BMP_INFO_SIZE + img_size;
	header.bfReserved = 0;
	header.bfOffBits = BMP_HEADER_SIZE + BMP_INFO_SIZE;
	bmp_Info info;
	memset(&info, 0, sizeof(info));
	info.biSize = BMP_INFO_SIZE; 
	info.biWidth = img->w;
	info.biHeight = img->h;
	info.biPlanes = 1;
	info.biBitCount = 24;
	info.biCompression = 0;
	info.biSizeImage = img_size;
	info.biXPelsPerMeter = 0;
	info.biYPelsPerMeter = 0;
	info.biClrUsed = 0;
	info.biClrImportant = 0;
	bytes_written += fwrite(&header, 1, BMP_HEADER_SIZE, f);
	bytes_written += fwrite(&info, 1, BMP_INFO_SIZE, f);
	char fill[4];
	if(padding > 0)
		memset(fill, 0, padding);
	/* writing image data */
	for(int y = img->h-1; y >= 0; y--)
	{
		int base = y*img->w;
		for(int x = 0; x < img->w; x++)
		{
			/* writing RGB component */
			bytes_written += fwrite(&img->pixels[base+x], 1, 3, f);
		}
		/* writing padding */
		bytes_written += fwrite(fill, 1, padding, f);
	}

	fclose(f);
	return bytes_written;
}

void bmp_free(bmp_Image * img)
{
	if(img != NULL)
	{
		free(img->pixels);
		free(img);
	}
}
