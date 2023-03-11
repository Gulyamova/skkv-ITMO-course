#define _CRT_SECURE_NO_WARNINGS
#include "return_codes.h"
#include <stdio.h>
#include <stdlib.h>

#define LIBDEFLATE 

//#define ISAL 

//#define ZLIB 

#ifdef ZLIB

#include <zlib.h>

#elif defined(LIBDEFLATE)

#include <libdeflate.h>

#elif defined(ISAL)

#include <include/igzip_lib.h>

#else
#error ERROR LIB
#endif

int modulus(int x, int y)
{
	if (x > y)
		return x - y;
	return y - x;
}

void fill_line(unsigned char* output_image, int image_filter_type, long ind, int width, int image_color_type)
{
	int pix_byte;
	if (image_color_type == 0)
		pix_byte = 1;
	else
		pix_byte = 3;

	for (int i = 1; i <= width; i++)
	{
		int arr_index = ind * (width + 1) + i;

		int first;
		if (i > pix_byte)
			first = output_image[arr_index - pix_byte];
		else
			first = 0;

		int second;
		if (ind > 0)
			second = output_image[(ind - 1) * (width + 1) + i];
		else
			second = 0;

		int third;
		if (i > pix_byte && ind > 0)
			third = output_image[(ind - 1) * (width + 1) + i - pix_byte];
		else
			third = 0;

		if (image_filter_type == 0)
		{
			continue;
		}
		else if (image_filter_type == 1)
		{
			output_image[arr_index] += first;
		}
		else if (image_filter_type == 2)
		{
			output_image[arr_index] += second;
		}
		else if (image_filter_type == 3)
		{
			output_image[arr_index] += (first + second) / 2;
		}
		else if (image_filter_type == 4)
		{
			int combine = first + second - third;
			int comb_first = modulus(combine, first);
			int comb_second = modulus(combine, second);
			int comb_third = modulus(combine, third);
			if (comb_first <= comb_second && comb_first <= comb_third)
			{
				combine = first;
			}
			else if (comb_second <= comb_third)
			{
				combine = second;
			}
			else
			{
				combine = third;
			}
			output_image[arr_index] += combine;
		}
		else
		{
			fprintf(stderr, "something wrong with uncompressed data\n");
		}
	}
}

int bytes_to_int(unsigned char* bytes)
{
	int pow[4] = { 1, 256, 65536, 16777216 };

	int num = 0;

	for (int i = 0; i < 4; i++)
	{
		num += pow[3 - i] * bytes[i];
	}
	return num;
}

long IDAT_chunk_sizes(unsigned char* image_bytes)
{
	long curr_index = 33L;
	long IDAT_chunk_size = 0L;

	unsigned char buffer[4] = { 0,0,0,0 };

	bool IEND = false;

	while (!IEND) {

		long chunk_size = 0L;

		for (int i = 0; i < 4; i++) buffer[i] = image_bytes[curr_index + i];

		int pow[4] = { 1, 256, 65536, 16777216 };
		for (int i = 0; i < 4; i++) chunk_size += pow[3 - i] * buffer[i];

		for (int i = 0; i < 4; i++) buffer[i] = image_bytes[curr_index + 4 + i];

		bool IDAT = true;
		char  idat[4] = { 'I', 'D', 'A', 'T' };
		for (int i = 0; i < 4; i++) {
			if (idat[i] != buffer[i]) {
				IDAT = false;
			}
		}

		if (IDAT)
		{
			IDAT_chunk_size += chunk_size;
		}

		curr_index += chunk_size + 12;


		IEND = true;
		char  iend[4] = { 'I', 'E', 'N', 'D' };
		for (int i = 0; i < 4; i++) {
			if (iend[i] != buffer[i]) {
				IEND = false;
			}
		}
	}

	return IDAT_chunk_size;
}

void fill_IDAT_chunks(unsigned char* image_bytes, unsigned char* filling)
{
	long curr_index = 33L;
	long IDAT_size = 0L;

	unsigned char buffer[4] = { 0,0,0,0 };
	bool IEND = false;

	while (!IEND) {
		long chunk_size = 0L;

		for (int i = 0; i < 4; i++) buffer[i] = image_bytes[curr_index + i];

		int pow[4] = { 1, 256, 65536, 16777216 };
		for (int i = 0; i < 4; i++) chunk_size += pow[3 - i] * buffer[i];

		for (int i = 0; i < 4; i++) buffer[i] = image_bytes[curr_index + 4 + i];


		bool IDAT = true;
		char  idat[4] = { 'I', 'D', 'A', 'T' };
		for (int i = 0; i < 4; i++) {
			if (idat[i] != buffer[i]) {
				IDAT = false;
			}
		}

		if (IDAT)
		{

			for (int i = 0; i < chunk_size; i++)
			{
				filling[IDAT_size + i] = image_bytes[curr_index + 8 + i];
			}

			IDAT_size += chunk_size;
		}

		curr_index += chunk_size + 12;


		IEND = true;
		char  iend[4] = { 'I', 'E', 'N', 'D' };
		for (int i = 0; i < 4; i++) {
			if (iend[i] != buffer[i]) {
				IEND = false;
			}
		}

	}
}


int main(int argc, char* argv[]) {
	if (argc != 3) {
		fprintf(stderr, "wrong number of input arguments\n");
		return ERROR_INVALID_PARAMETER;
	}

	FILE* image = fopen(argv[1], "rb");
	if (image == NULL) {
		fprintf(stderr, "wrong input image\n");
		fclose(image);
		return ERROR_FILE_EXISTS;
	}

	fseek(image, 0, SEEK_END);
	long image_size = ftell(image);
	rewind(image);

	unsigned char* image_bytes = (unsigned char*)malloc(sizeof(unsigned char) * image_size);
	if (image_bytes == NULL)
	{
		fclose(image);
		fprintf(stderr, "can't allocate memory\n");
		return ERROR_NOT_ENOUGH_MEMORY;
	}

	long res = fread(image_bytes, 1, image_size, image);

	if (res != image_size)
	{
		free(image_bytes);
		fclose(image);
		fprintf(stderr, "file can't read\n");
		return ERROR_UNKNOWN;
	}
	fclose(image);

	bool has_magic = true;
	unsigned char magic_number[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };
	for (int i = 0; i < 8 && i < image_size; i++) {
		if (image_bytes[i] != magic_number[i]) {
			has_magic = false;
		}
	}

	if (image_size < 8 || !has_magic)
	{
		free(image_bytes);
		fprintf(stderr, "input image type not PNG!\n");
		return ERROR_INVALID_DATA;
	}

	unsigned char sub_bytes[4];

	for (int i = 16; i < 20; i++) sub_bytes[i - 16] = image_bytes[i];
	int width = bytes_to_int(sub_bytes);

	for (int i = 20; i < 24; i++) sub_bytes[i - 20] = image_bytes[i];
	int height = bytes_to_int(sub_bytes);

	int image_color_type = image_bytes[25];

	if (image_color_type != 2 && image_color_type != 0)
	{
		free(image_bytes);
		fprintf(stderr, "wrong image color type\n");
		return ERROR_INVALID_DATA;
	}

	long IDAT_chunks_size = IDAT_chunk_sizes(image_bytes);

	unsigned char* IDAT_data = (unsigned char*)malloc(sizeof(unsigned char) * IDAT_chunks_size);
	if (IDAT_data == NULL) {
		free(image_bytes);
		free(IDAT_data);
		fprintf(stderr, "can't allocate memory\n");
		return ERROR_NOT_ENOUGH_MEMORY;
	}

	fill_IDAT_chunks(image_bytes, IDAT_data);

	long output_image_size = (long)width * height * 10;
	unsigned char* output_image = (unsigned char*)malloc(output_image_size * sizeof(unsigned char));
	if (output_image == NULL) {
		free(image_bytes);
		free(IDAT_data);
		free(output_image);
		fprintf(stderr, "can't allocate memory\n");
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	free(image_bytes);

#ifdef ZLIB
	uLong zlib_output_image_size = output_image_size;
	int uncompress_result = uncompress(output_image, &zlib_output_image_size, IDAT_data, IDAT_chunks_size);

	if (uncompress_result == Z_DATA_ERROR)
	{
		free(IDAT_data);
		free(output_image);
		fprintf(stderr, "smth wrong with image data\n");
		return ERROR_INVALID_DATA;
	}
	else if (uncompress_result == Z_MEM_ERROR)
	{
		free(IDAT_data);
		free(output_image);
		fprintf(stderr, "memory error\n");
		return ERROR_NOT_ENOUGH_MEMORY;
	}

#endif

#ifdef ISAL
	struct inflate_state uncompress;
	isal_inflate_init(&uncompress);
	uncompress.next_in = IDAT_data;
	uncompress.avail_out = output_image_size;
	uncompress.crc_flag = IGZIP_ZLIB;
	uncompress.avail_in = IDAT_chunks_size;
	uncompress.next_out = output_image;
	int uncompress_result = isal_inflate(&uncompress);

	if (uncompress_result == ISAL_OUT_OVERFLOW)
	{
		free(IDAT_data);
		free(output_image);
		fprintf(stderr, "memory error\n");
		return ERROR_NOT_ENOUGH_MEMORY;
	}

#endif

#ifdef LIBDEFLATE
	size_t p = (size_t)width * height * 10;
	int uncompress_result = libdeflate_zlib_decompress(libdeflate_alloc_decompressor(), IDAT_data, IDAT_chunks_size, output_image, output_image_size, &p);

	if (uncompress_result == LIBDEFLATE_INSUFFICIENT_SPACE)
	{
		free(IDAT_data);
		free(output_image);
		fprintf(stderr, "memory error\n");
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	else if (uncompress_result == LIBDEFLATE_BAD_DATA)
	{
		free(IDAT_data);
		free(output_image);
		fprintf(stderr, "smth wrong with image data\n");
		return ERROR_INVALID_DATA;
	}
#endif

	free(IDAT_data);

	if (uncompress_result != 0)
	{
		fprintf(stderr, "unknown error while uncompress image");
		free(output_image);
		return ERROR_UNKNOWN;
	}

	
	FILE* output_image_file = fopen(argv[2], "wb");
	if (output_image_file == NULL)
	{
		free(output_image);
		fprintf(stderr, "can't open or create output image file\n");
		return ERROR_NOT_FOUND;
	}

	if (image_color_type == 0)
		fprintf(output_image_file, "P5\n");
	else
		fprintf(output_image_file, "P6\n");
	fprintf(output_image_file, "%d %d\n", width, height);
	fprintf(output_image_file, "255\n");


	width *= (image_color_type == 0) ? 1 : 3;
	for (int i = 0; i < height; i++)
	{
		int image_filter_type = output_image[i * (width + 1)];
		fill_line(output_image, image_filter_type, (long)i, width, image_color_type);
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			putc(output_image[(long)i * (width + 1) + (j + 1)], output_image_file);
		}
	}
	fclose(output_image_file);
	free(output_image);
}