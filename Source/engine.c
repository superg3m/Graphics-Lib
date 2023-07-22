#include "../Header/engine.h"

bool RGB_READABLE = true;
bool MONOCHROMATIC = false; // If this is true, RGB_READABLE must be false

void intToCharArray(char *characterArr, int value) {
  // Determine the number of digits in the integer
  int numDigits = snprintf(NULL, 0, "%d", value);

  // Convert the integer to a character array using sprintf
  snprintf(characterArr, numDigits + 1, "%d", value);
}

void fill(Pixel *pixels, size_t width, size_t height, Pixel pixel) {
  for (size_t i = 0; i < width * height; ++i) {
    pixels[i] = pixel;
  }
}

#define return_defer(value)                                                    \
  do {                                                                         \
    result = (value);                                                          \
    goto defer;                                                                \
  } while (0)

// Save the Pixel data to a PPM file
int save_to_ppm_file(Pixel *pixels, size_t width, size_t height,
                     const char *file_path) {
  int result = 0;
  FILE *f = NULL;
  const char *ppm_file_path = "../ppm/";

  // Allocate a character array to hold the final file path
  char final_file_path[256]; // Adjust the size based on your requirements

  // Copy the initial part of the path into the final file path
  strcpy(final_file_path, ppm_file_path);

  // Concatenate the file path to the final file path
  strcat(final_file_path, file_path);

  f = fopen(final_file_path, "wb");
  if (f == NULL)
    return_defer(-1);

  const char *ppmFileFormat;
  if (RGB_READABLE) {
    ppmFileFormat = "P3\n";
  } else if (MONOCHROMATIC) {
    ppmFileFormat = "P6\n";
  } else {
    ppmFileFormat = "P6\n";
  }

  fprintf(f, "%s%zu %zu 255\n", ppmFileFormat, width, height);
  if (ferror(f))
    return_defer(-1);

  // Increase the size of the 'str' array to accommodate larger numbers
  char str[3][6]; // A 2D array to store the string representations of each
                  // channel (5 digits + 1 null terminator)

  for (size_t i = 0; i < width * height; ++i) {
    Pixel pixel = pixels[i];

    if (RGB_READABLE) {
      intToCharArray(str[0], pixel.redChannel);
      intToCharArray(str[1], pixel.greenChannel);
      intToCharArray(str[2], pixel.blueChannel);

      // Null-terminate the strings to avoid any issues
      str[0][5] = '\0';
      str[1][5] = '\0';
      str[2][5] = '\0';

      // Write the formatted string representations of each channel to the file
      fprintf(f, "%s %s %s\n", str[0], str[1], str[2]);
      if (ferror(f)) {
        fprintf(stderr, "ERROR: writing to file %s: %s\n", final_file_path,
                strerror(errno));
        fclose(f);
        return -1;
      }
    } else if (MONOCHROMATIC) {
      fwrite(&pixel, sizeof(Pixel), 1, f);

      if (ferror(f)) {
        fprintf(stderr, "ERROR: writing to file %s: %s\n", final_file_path,
                strerror(errno));
        fclose(f);
        return -1;
      }
    } else {
      fwrite(&pixel.redChannel, sizeof(pixel.redChannel), 1, f);
      fwrite(&pixel.greenChannel, sizeof(pixel.greenChannel), 1, f);
      fwrite(&pixel.blueChannel, sizeof(pixel.blueChannel), 1, f);
      if (ferror(f)) {
        fprintf(stderr, "ERROR: writing to file %s: %s\n", final_file_path,
                strerror(errno));
        fclose(f);
        return -1;
      }
    }
  }

defer:
  if (f)
    fclose(f);
  return result;
}

void fill_rect(Pixel *pixels, size_t pixels_width, size_t pixels_height, int x0,
               int y0, size_t w, size_t h, Pixel pixel) {
  for (int dy = 0; dy < (int)h; ++dy) {
    int y = y0 + dy;
    if (0 <= y && y < (int)pixels_height) {
      for (int dx = 0; dx < (int)w; ++dx) {
        int x = x0 + dx;
        if (0 <= x && x < (int)pixels_width) {
          pixels[y * pixels_width + x] = pixel;
        }
      }
    }
  }
}

void fill_circle(Pixel *pixels, size_t pixels_width, size_t pixels_height,
                 int cx, int cy, int r, Pixel pixel) {
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
            pixels[y * pixels_width + x] = pixel;
          }
        }
      }
    }
  }
}
