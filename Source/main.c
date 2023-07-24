#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../Header/engine.h"

#define WIDTH 800
#define HEIGHT 600

#define COLS (8 * 2)
#define ROWS (6 * 2)
#define CELL_WIDTH (WIDTH / COLS)
#define CELL_HEIGHT (HEIGHT / ROWS)

static uint32_t pixels[WIDTH * HEIGHT];

bool checker_example(void) {
  fill_canvas(pixels, WIDTH, HEIGHT, BLACK_COLOR);

  for (int y = 0; y < ROWS; ++y) {
    for (int x = 0; x < COLS; ++x) {
      uint32_t color = BLACK_COLOR;
      if ((x + y) % 2 == 0) {
        color = RED_COLOR;
      }
      fill_rect(pixels, WIDTH, HEIGHT, x * CELL_WIDTH, y * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, color);
    }
  }

  const char *file_path = "../ppm/checker.ppm";
  Errno err = save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
  if (err) {
    fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
    return false;
  }

  return true;
}

float lerpf(float a, float b, float t) { return a + (b - a) * t; }

bool circle_example(void) {
  fill_canvas(pixels, WIDTH, HEIGHT, BLACK_COLOR);

  for (int y = 0; y < ROWS; ++y) {
    for (int x = 0; x < COLS; ++x) {
      float u = (float)x / COLS;
      float v = (float)y / ROWS;
      float t = (u + v) / 2;

      size_t radius = CELL_WIDTH;
      if (CELL_HEIGHT < radius) radius = CELL_HEIGHT;

      fill_circle(pixels, WIDTH, HEIGHT, x * CELL_WIDTH + CELL_WIDTH / 2, y * CELL_HEIGHT + CELL_HEIGHT / 2, (size_t)lerpf(radius / 8, radius / 2, t), RED_COLOR);
    }
  }

  const char *file_path = "../ppm/circle.ppm";
  Errno err = save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
  if (err) {
    fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
    return false;
  }
  return true;
}

bool lines_example(void) {
  const char *file_path = "../ppm/lines.ppm";

  fill_canvas(pixels, WIDTH, HEIGHT, BLACK_COLOR);

  BrensenhamsLineAlgorithm(pixels, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, RED_COLOR);

  BrensenhamsLineAlgorithm(pixels, WIDTH, HEIGHT, WIDTH, 0, 0, HEIGHT, RED_COLOR);

  BrensenhamsLineAlgorithm(pixels, WIDTH, HEIGHT, 0, 0, WIDTH / 4, HEIGHT, GREEN_COLOR);

  BrensenhamsLineAlgorithm(pixels, WIDTH, HEIGHT, WIDTH / 4, 0, 0, HEIGHT, GREEN_COLOR);

  BrensenhamsLineAlgorithm(pixels, WIDTH, HEIGHT, WIDTH, 0, WIDTH / 4 * 3, HEIGHT, GREEN_COLOR);

  BrensenhamsLineAlgorithm(pixels, WIDTH, HEIGHT, WIDTH / 4 * 3, 0, WIDTH, HEIGHT, GREEN_COLOR);

  BrensenhamsLineAlgorithm(pixels, WIDTH, HEIGHT, 0, HEIGHT / 2, WIDTH, HEIGHT / 2, BLUE_COLOR);

  BrensenhamsLineAlgorithm(pixels, WIDTH, HEIGHT, WIDTH / 2, 0, WIDTH / 2, HEIGHT, BLUE_COLOR);

  Errno err = save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
  if (err) {
    fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
    return false;
  }
  return true;
}

bool triangle_example(void) {
  fill_canvas(pixels, WIDTH, HEIGHT, BLACK_COLOR);

  draw_triangle(pixels, WIDTH, HEIGHT, WIDTH / 2, 0, (WIDTH / 2) - 50, HEIGHT / 2, (WIDTH / 2) + 50, HEIGHT / 2, RED_COLOR);

  draw_triangle(pixels, WIDTH, HEIGHT, WIDTH / 2, HEIGHT, (WIDTH / 2) - 50, HEIGHT / 2, (WIDTH / 2) + 50, HEIGHT / 2, GREEN_COLOR);

  draw_triangle(pixels, WIDTH, HEIGHT, WIDTH / 4, 200, (WIDTH / 4) - 50, HEIGHT / 2, (WIDTH / 4) + 50, HEIGHT / 2, WHITE_COLOR);

  draw_triangle(pixels, WIDTH, HEIGHT, WIDTH / 4 * 3, 200, (WIDTH / 4 * 3) - 50, HEIGHT / 2, (WIDTH / 4 * 3) + 50, HEIGHT / 2, WHITE_COLOR);

  const char *file_path = "../ppm/triangle.ppm";
  Errno err = save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
  if (err) {
    fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
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
