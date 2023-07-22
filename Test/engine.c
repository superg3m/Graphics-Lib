#include "../Header/engine.h"

void intToCharArray(char *characterArr, int value) {
  // Determine the number of digits in the integer
  int numDigits = snprintf(NULL, 0, "%d", value);

  // Convert the integer to a character array using sprintf
  snprintf(characterArr, numDigits + 1, "%d", value);
}

void fill_canvas(uint32_t *pixels, size_t width, size_t height, uint32_t color) {
  for (size_t i = 0; i < width * height; ++i) {
    pixels[i] = color;
  }
}

Errno save_to_ppm_file(uint32_t *pixels, size_t width, size_t height, const char *file_path)
{
  int result = 0;
  FILE *f = NULL;
  f = fopen(file_path, "wb");
  if (f == NULL) return_defer(errno);

  fprintf(f, "P6\n%zu %zu 255\n", width, height);
  if (ferror(f)) return_defer(errno);

  for (size_t i = 0; i < width*height; ++i) {
    uint32_t pixel = pixels[i];
    uint8_t bytes[3] = {
      (uint8_t)(pixel>>(8*0))&0xFF,
      (uint8_t)(pixel>>(8*1))&0xFF,
      (uint8_t)(pixel>>(8*2))&0xFF,
    };
    fwrite(bytes, sizeof(bytes), 1, f);
    if (ferror(f)) return_defer(errno);
  }

defer:
  if (f) fclose(f);
  return result;
}

void fill_rect(uint32_t *pixels, size_t pixels_width, size_t pixels_height, int x0,
               int y0, size_t w, size_t h, uint32_t color) {
  for (int dy = 0; dy < (int)h; ++dy) {
    int y = y0 + dy;
    if (0 <= y && y < (int)pixels_height) {
      for (int dx = 0; dx < (int)w; ++dx) {
        int x = x0 + dx;
        if (0 <= x && x < (int)pixels_width) {
          pixels[y * pixels_width + x] = color;
        }
      }
    }
  }
}

void fill_circle(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
                 int cx, int cy, int r, uint32_t color) {
  int x1 = cx - r;
  int y1 = cy - r;
  int x2 = cx + r;
  int y2 = cy + r;
  for (int y = y1; y <= y2; ++y) {
    if (0 <= y && y < (int)pixels_height) {
      for (int x = x1; x <= x2; ++x) {
        if (0 <= x && x < (int)pixels_width) {
          int dx = x - cx;
          int dy = y - cy;
          if (dx * dx + dy * dy <= r * r) {
            pixels[y * pixels_width + x] = color;
          }
        }
      }
    }
  }
}

void BrensenhamsLineAlgorithm(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
                   int x0, int y0, int x1, int y1, uint32_t color) {
  if (pixels_width <= 0 || pixels_height <= 0) {
    fprintf(stderr, "ERROR: invalid dimensions %zux%zu\n", pixels_width, pixels_height);
    return;
  }

  int dx = abs(x1 - x0);
  int xStep = x0 < x1 ? 1 : -1;

  int dy = -abs(y1 - y0);
  int yStep = y0 < y1 ? 1 : -1;

  int error = dx + dy;

  int currentX = x0;
  int currentY = y0;

  while (true) {
    // Check if the current coordinates are within the bounds of the pixels array
    if (currentX >= 0 && currentX < (int)pixels_width && currentY >= 0 && currentY < (int)pixels_height) {
      // Calculate the index and limit it to the maximum valid index
      size_t index = currentY * pixels_width + currentX;
      index = index >= (pixels_width * pixels_height) ? (pixels_width * pixels_height) - 1 : index;
      // Write the color to the pixels array
      pixels[index] = color;
    }

    if (currentX == x1 && currentY == y1)
      break;
    int e2 = 2 * error;

    if (e2 >= dy) {
      if (currentX == x1)
          break;
      error += dy;
      currentX += xStep;
    }

    if (e2 <= dx) {
      if (currentY == y1)
          break;
      error += dx;
      currentY += yStep;
    }
  }
}

void draw_triangle(uint32_t* pixels, int pixel_width, int pixel_height, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
  BrensenhamsLineAlgorithm(pixels, pixel_width, pixel_height, x0, y0, x1, y1, color);
  BrensenhamsLineAlgorithm(pixels, pixel_width, pixel_height, x1, y1, x2, y2, color);
  BrensenhamsLineAlgorithm(pixels, pixel_width, pixel_height, x2, y2, x0, y0, color);
}

void swap_int(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}