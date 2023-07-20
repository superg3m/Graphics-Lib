#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <string.h>

typedef struct Pixel {
    uint8_t redChannel;
    uint8_t greenChannel;
    uint8_t blueChannel;
    uint8_t alphaChannel;
} Pixel;

// Define a helper macro for creating a Pixel instance
#define PIXEL_INSTANCE(r, g, b, a) (Pixel){(r), (g), (b), (a)}

// Define the color macros using the PIXEL macro
#define BLACK_COLOR (Pixel){20, 20, 20, 255}
#define RED_COLOR (Pixel){255, 20, 20, 255}

#define BLUE_COLOR (Pixel){20, 20, 255, 255}
#define GREEN_COLOR (Pixel){20, 255, 20, 255}

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#endif // TYPES_H
