#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "file.h"
#include "str.h"

int write_image(String *fnm, int width, int height, int *values) {
  /* values[0:3 * width * height] take floats in range [0, 255]*/
  FILE *fp = fopen(string_str(fnm), "wb");
  if (fp == NULL)
    printf("Could not open file %s\n", string_str(fnm));
  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png == NULL)
    printf("Could not initialize png\n");
  png_infop info = png_create_info_struct(png);
  if (info == NULL)
    printf("Could not initialize png info\n");
  if (setjmp(png_jmpbuf(png)))
    abort();
  png_init_io(png, fp);
  png_set_IHDR(png, info, width, height,
               8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  png_write_info(png, info);
  png_bytep buffer = malloc(3 * width * sizeof(png_byte));
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++)
      for (int c = 0; c < 3; c++)
        buffer[x * 3 + c] = values[3 * y * width + 3 * x + c];
    png_write_row(png, buffer);
   }
  png_write_end(png, NULL);
  fclose(fp);
  png_free_data(png, info, PNG_FREE_ALL, -1);
  png_destroy_write_struct(&png, (png_infopp) NULL);
  free(buffer);
}

int file_len(String *fnm) {
  int len = 0;
  char c;
  FILE *file = fopen(string_str(fnm), "r");
  while ((c = getc(file)) != EOF)
    len++;
  fclose(file);
  return len;
}

String * read_file(String *fnm) {
  String *str = string_alloc_str(file_len(fnm));
  char c;
  int i = 0;
  FILE *file = fopen(string_str(fnm), "r");
  while ((c = getc(file)) != EOF)
    string_set_char(str, c, i++);
  return str;
}
