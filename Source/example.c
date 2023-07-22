#include "../Header/engine.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600

#define COLS (8 * 2)
#define ROWS (6 * 2)
#define CELL_WIDTH (WIDTH / COLS)
#define CELL_HEIGHT (HEIGHT / ROWS)

static Pixel pixels[WIDTH * HEIGHT];

void swap_int(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

#define accessArrayMemory(array, arrayName, elementSize, index) \
  ({ \
    if (index < 0 || index >= (sizeof(array) / elementSize)) { \
      fprintf(stderr, "ERROR: index %d out of bounds for array %s\n", index, arrayName); \
      assert(index >= 0 && index < (sizeof(array) / elementSize)); \
    } \
    array[index]; \
  })

#define writeArrayMemory(array, arrayName, element, index) \
  do { \
    if (index < 0 || index >= (sizeof(array) / sizeof(element))) { \
      fprintf(stderr, "ERROR: index %d out of bounds for array %s\n", index, arrayName); \
      assert(index >= 0 && index < (sizeof(array) / sizeof(element))); \
    } \
    array[index] = element; \
  } while (0)


void BrensenhamsLineAlgorithm(int x0, int y0, int x1, int y1, Pixel pixel) {
  if (WIDTH <= 0 || HEIGHT <= 0) {
    fprintf(stderr, "ERROR: invalid dimensions %zux%zu\n", WIDTH, HEIGHT);
    return;
  }

  if (x0 >= WIDTH) {
    x0 = WIDTH - 1;
  }
  if (x1 >= WIDTH) {
    x1 = WIDTH - 1;
  }

  if (y0 >= HEIGHT) {
    y0 = HEIGHT - 1;
  }
  if (y1 >= HEIGHT) {
    y1 = HEIGHT - 1;
  }
  /*
  int dx = x1 - x0;
  int dy = y1 - y0;

  if (dx != 0) {
    int c = y0 - dy*x0/dx;
    if (x0 > x1) swap_int(&x0, &x1);
    for (int x = x0; x <= x1; ++x) {
      if (0 <= x && x < (int)WIDTH) {
        int sy0 = dy*x/dx + c;
        int sy1 = dy*(x + 1)/dx + c;
        if (sy0 > sy1) swap_int(&sy0, &sy1);
        for (int y = sy0; y <= sy1; ++y) {
          if (0 <= y && y < (int)HEIGHT) {
            pixels[y*WIDTH + x] = pixel;
          }
        }
      }
    }
  } else {
      int x = x1;
      if (0 <= x && x < (int)WIDTH) {
        if (y0 > y1) swap_int(&y0, &y1);
        for (int y = y0; y <= y1; ++y) {
          if (0 <= y && y < (int)HEIGHT) {
            pixels[y*WIDTH + x] = pixel;
          }
        }
      }
  }
  */
  int dx = abs(x1 - x0);
  int xStep = x0 < x1 ? 1 : -1;

  int dy = -abs(y1 - y0);
  int yStep = y0 < y1 ? 1 : -1;

  int error = dx + dy;

  int currentX = x0;
  int currentY = y0;
  
  while (true) {
    writeArrayMemory(pixels, "pixels", pixel, currentY * (int)WIDTH + currentX);
    //save_to_ppm_file(pixels, WIDTH, HEIGHT, "lines.ppm");
    if (currentX == x1 && y0 == y1) break;
    int e2 = 2 * error;

    if (e2 >= dy) {
      if (x0 == x1) break;
      error += dy;
      currentX += xStep;
    }

    if (e2 <= dx) {
      if (currentY == y1) break;
      error += dx;
      currentY += yStep; 
    }
  }
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, Pixel pixel) {
  BrensenhamsLineAlgorithm(x0, y0, x1, y1, pixel);
  BrensenhamsLineAlgorithm(x1, y1, x2, y2, pixel);
  BrensenhamsLineAlgorithm(x2, y2, x0, y0, pixel);
}

bool checker_example(void) {
  fill(pixels, WIDTH, HEIGHT, BLACK_COLOR);

  for (int y = 0; y < ROWS; ++y) {
    for (int x = 0; x < COLS; ++x) {
      Pixel color = BLACK_COLOR;
      if ((x + y) % 2 == 0) {
        color = RED_COLOR;
      }
      fill_rect(pixels, WIDTH, HEIGHT, x * CELL_WIDTH, y * CELL_HEIGHT,
                CELL_WIDTH, CELL_HEIGHT, color);
    }
  }

  const char *file_path = "checker.ppm";
  Errno err = save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
  if (err) {
    fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path,
            strerror(errno));
    return false;
  }

  return true;
}

float lerpf(float a, float b, float t) { return a + (b - a) * t; }

bool circle_example(void) {
  fill(pixels, WIDTH, HEIGHT, BLACK_COLOR);

  for (int y = 0; y < ROWS; ++y) {
    for (int x = 0; x < COLS; ++x) {
      float u = (float)x / COLS;
      float v = (float)y / ROWS;
      float t = (u + v) / 2;

      size_t radius = CELL_WIDTH;
      if (CELL_HEIGHT < radius)
        radius = CELL_HEIGHT;

      fill_circle(pixels, WIDTH, HEIGHT, x * CELL_WIDTH + CELL_WIDTH / 2,
                  y * CELL_HEIGHT + CELL_HEIGHT / 2,
                  (size_t)lerpf(radius / 8, radius / 2, t), RED_COLOR);
    }
  }

  const char *file_path = "circle.ppm";
  Errno err = save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
  if (err) {
    fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path,
            strerror(errno));
    return false;
  }
  return true;
}

bool lines_example(void) {
  const char *file_path = "lines.ppm";
  fill(pixels, WIDTH, HEIGHT, BLACK_COLOR);

  BrensenhamsLineAlgorithm(0, 0, WIDTH, HEIGHT, RED_COLOR);

  BrensenhamsLineAlgorithm(WIDTH, 0, 0, HEIGHT, RED_COLOR);

  BrensenhamsLineAlgorithm(0, 0, WIDTH / 4, HEIGHT, GREEN_COLOR);

  BrensenhamsLineAlgorithm(WIDTH / 4, 0, 0, HEIGHT, GREEN_COLOR);

  BrensenhamsLineAlgorithm(WIDTH, 0, WIDTH / 4 * 3, HEIGHT, GREEN_COLOR);

  BrensenhamsLineAlgorithm(WIDTH / 4 * 3, 0, WIDTH, HEIGHT, GREEN_COLOR);

  BrensenhamsLineAlgorithm(0, HEIGHT / 2, WIDTH, HEIGHT / 2, BLUE_COLOR);

  BrensenhamsLineAlgorithm(WIDTH / 2, 0, WIDTH / 2, HEIGHT, BLUE_COLOR);
  
  
  Errno err = save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
  if (err) {
    fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path,
            strerror(errno));
    return false;
  }
  return true;
}

bool triangle_example(void) {
  fill(pixels, WIDTH, HEIGHT, BLACK_COLOR);

  draw_triangle(WIDTH / 2, 0, (WIDTH / 2) - 50, HEIGHT / 2, (WIDTH / 2) + 50, HEIGHT / 2, RED_COLOR);
  draw_triangle(WIDTH / 2, HEIGHT, (WIDTH / 2) - 50, HEIGHT / 2, (WIDTH / 2) + 50, HEIGHT / 2, GREEN_COLOR);

  draw_triangle(WIDTH / 4, 200, (WIDTH / 4) - 50, HEIGHT / 2, (WIDTH / 4) + 50, HEIGHT / 2, WHITE_COLOR);

  draw_triangle(WIDTH / 4 * 3, 200, (WIDTH / 4 * 3) - 50, HEIGHT / 2, (WIDTH / 4 * 3) + 50, HEIGHT / 2, WHITE_COLOR);
  
  const char *file_path = "triangle.ppm";
  Errno err = save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
  if (err) {
    fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path,
            strerror(errno));
    return false;
  }
  return true;
}

int main(void) {
  if (!checker_example()) {
    return -1;
  }
    
  if (!circle_example()) {
    return -1;
  }
    
  if (!lines_example()) {
    return -1;
  }

  if (!triangle_example()) {
    return -1;
  }
  return 0;
}
