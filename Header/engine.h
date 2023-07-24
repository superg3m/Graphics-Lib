#ifndef ENGINE_H
#define ENGINE_H

#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "types.h"

void intToCharArray(char *characterArr, int value);

void fill_canvas(uint32_t *pixels, size_t width, size_t height, uint32_t uint32_t);

typedef int Errno;

#define return_defer(value) \
  do {                      \
    result = (value);       \
    goto defer;             \
  } while (0)

Errno save_to_ppm_file(uint32_t *pixels, size_t width, size_t height, const char *file_path);

void fill_rect(uint32_t *pixels, size_t pixels_width, size_t pixels_height, int x0, int y0, size_t w, size_t h, uint32_t color);

void fill_circle(uint32_t *pixels, size_t pixels_width, size_t pixels_height, int cx, int cy, int r, uint32_t color);

void swap_int(int *a, int *b);

void BrensenhamsLineAlgorithm(uint32_t *pixels, int pixels_width, int pixels_height, int x0, int y0, int x1, int y1, uint32_t uint32_t);

void draw_triangle(uint32_t *pixels, int pixel_width, int pixel_height, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t uint32_t);

#endif
