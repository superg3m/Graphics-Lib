#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include "types.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>

void intToCharArray(char* characterArr, int value);

void fill(Pixel *pixels, size_t width, size_t height, Pixel color);

typedef int Errno;

#define return_defer(value) do { result = (value); goto defer; } while (0)

Errno save_to_ppm_file(Pixel *pixels, size_t width, size_t height, const char *file_path);

void fill_rect(Pixel *pixels, size_t pixels_width, size_t pixels_height,
                      int x0, int y0, size_t w, size_t h,
                      Pixel color);

void fill_circle(Pixel *pixels, size_t pixels_width, size_t pixels_height,
                        int cx, int cy, int r,
                        Pixel color);

#endif
