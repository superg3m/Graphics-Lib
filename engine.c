#include "engine.h"
 
bool RGB_READABLE = false;
bool MONOCHROMATIC = true; // If this is true, RGB_READABLE must be false

void intToCharArray(char* characterArr, int value) {
    // Determine the number of digits in the integer
    int numDigits = snprintf(NULL, 0, "%d", value);

    // Convert the integer to a character array using sprintf
    snprintf(characterArr, numDigits + 1, "%d", value);
}

void fill(Pixel *pixels, size_t width, size_t height, Pixel pixel)
{
    for (size_t i = 0; i < width*height; ++i) {
        pixels[i] = pixel;
    }
}

typedef int Errno;

#define return_defer(value) do { result = (value); goto defer; } while (0)

Errno save_to_ppm_file(Pixel *pixels, size_t width, size_t height, const char *file_path)
{
    int result = 0;
    FILE *f = NULL;

    f = fopen(file_path, "wb");
    if (f == NULL) return_defer(errno);

    fprintf(f, "P6\n%d %d 255\n", width, height); // IF YOU WANT BINARY CHANGE TO - P6 - and then change back to p3
    if (ferror(f)) return_defer(errno);

    char str[3][4]; // A 2D array to store the string representations of each channel (3 digits + 1 null terminator)

    for (size_t i = 0; i < width * height; ++i) {
        Pixel pixel = pixels[i];
        
        if (RGB_READABLE) {
            intToCharArray(str[0], pixel.redChannel);
            intToCharArray(str[1], pixel.greenChannel);
            intToCharArray(str[2], pixel.blueChannel);

            // Null-terminate the strings to avoid any issues
            str[0][3] = '\0';
            str[1][3] = '\0';
            str[2][3] = '\0';
            
            // Write the formatted string representations of each channel to the file
            fprintf(f, "%s %s %s\n", str[0], str[1], str[2]);
            if (ferror(f)) {
                fprintf(stderr, "ERROR: writing to file %s: %s\n", file_path, strerror(errno));
                fclose(f);
                return -1;
            }
        } else if (MONOCHROMATIC) {
            fwrite(&pixel, sizeof(Pixel), 1, f);

            if (ferror(f)) {
                fprintf(stderr, "ERROR: writing to file %s: %s\n", file_path, strerror(errno));
                fclose(f);
                return -1;
            }
        } else {
            fwrite(&pixel.redChannel, sizeof(pixel.redChannel), 1, f);
            fwrite(&pixel.greenChannel, sizeof(pixel.greenChannel), 1, f);
            fwrite(&pixel.blueChannel, sizeof(pixel.blueChannel), 1, f);
            if (ferror(f)) {
                fprintf(stderr, "ERROR: writing to file %s: %s\n", file_path, strerror(errno));
                fclose(f);
                return -1;
            }
        } 
    }
defer:
    if (f) fclose(f);
    return result;
}

void fill_rect(Pixel *pixels, size_t pixels_width, size_t pixels_height,
                      int x0, int y0, size_t w, size_t h,
                      Pixel pixel)
{
    for (int dy = 0; dy < (int) h; ++dy) {
        int y = y0 + dy;
        if (0 <= y && y < (int) pixels_height) {
            for (int dx = 0; dx < (int) w; ++dx) {
                int x = x0 + dx;
                if (0 <= x && x < (int) pixels_width) {
                    pixels[y*pixels_width + x] = pixel;
                }
            }
        }
    }
}

void fill_circle(Pixel *pixels, size_t pixels_width, size_t pixels_height,
                        int cx, int cy, int r,
                        Pixel pixel)
{
    int x1 = cx - r;
    int y1 = cy - r;
    int x2 = cx + r;
    int y2 = cy + r;
    for (int y = y1; y <= y2; ++y) {
        if (0 <= y && y < (int) pixels_height) {
            for (int x = x1; x <= x2; ++x) {
                if (0 <= x && x < (int) pixels_width) {
                    int dx = x - cx;
                    int dy = y - cy;
                    if (dx*dx + dy*dy <= r*r) {
                        pixels[y*pixels_width + x] = pixel;
                    }
                }
            }
        }
    }
}