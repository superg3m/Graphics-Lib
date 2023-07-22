#include "../Header/engine.h"
#include <stdio.h>
#include "../SDL2/SDL.h"

#define WIDTH 800
#define HEIGHT 600

static uint32_t pixels[WIDTH * HEIGHT];
static float angle = 0;

float sqrtf(float x);
float atan2f(float y, float x);
float sinf(float x);
float cosf(float x);

#define PI 3.14159265359

void rotate_point(int *x, int *y, float angle)
{
    int dx = *x - WIDTH / 2;
    int dy = *y - HEIGHT / 2;
    float mag = sqrtf(dx * dx + dy * dy);
    float dir = atan2f(dy, dx) + angle;
    *x = cosf(dir) * mag + WIDTH / 2;
    *y = sinf(dir) * mag + HEIGHT / 2;
}

void render_triangle(uint32_t* pixels, float angle)
{
    fill_canvas(pixels, WIDTH, HEIGHT, BLACK_COLOR);
    int x1 = WIDTH / 2, y1 = HEIGHT / 8;
    int x2 = WIDTH / 8, y2 = HEIGHT / 2;
    int x3 = WIDTH * 7 / 8, y3 = HEIGHT * 7 / 8;

    // Rotate each point using the given angle
    rotate_point(&x1, &y1, angle);
    rotate_point(&x2, &y2, angle);
    rotate_point(&x3, &y3, angle);

    draw_triangle(pixels, WIDTH, HEIGHT, x1, y1, x2, y2, x3, y3, RED_COLOR);
    fill_rect(pixels, WIDTH, HEIGHT, x1 + 50, y1 + 50, 100, 100, WHITE_COLOR);
}

int initialize(SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **texture) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return 1;

    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
        SDL_Log("SDL_GetCurrentDisplayMode failed: %s", SDL_GetError());
        return -1;
    }

    int screen_width = dm.w;
    int screen_height = dm.h;

    // Calculate the window position to center it on the screen
    int window_x = (screen_width - WIDTH) / 2;
    int window_y = (screen_height - HEIGHT) / 2;

    *window = SDL_CreateWindow("Superg3m", window_x, window_y, WIDTH, HEIGHT, 0);
    if (*window == NULL)
        return 1;

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL)
        return 1;

    *texture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    if (*texture == NULL)
        return 1;

    return 0;
}

int main(int argc, char *argv[]) {
    int result = 0;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;

    if (initialize(&window, &renderer, &texture) != 0) {
        fprintf(stderr, "SDL initialization error: %s\n", SDL_GetError());
        result = 1;
    } else {
        Uint32 prev = SDL_GetTicks();
        Uint32 curr;

        for (;;) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        goto cleanup;
                        break;
                    case SDL_WINDOWEVENT:
                        if (event.window.event == SDL_WINDOWEVENT_MOVED) {
                            // Ignore window move events
                        }
                        break;
                    default:
                        break;
                }
            }

            // Compute Delta Time and increment the angle
            curr = SDL_GetTicks();
            float dt = (curr - prev) / 1000.0f;
            prev = curr;
            angle += 0.5f * PI * dt;

            // Render the texture with the updated angle
            render_triangle(pixels, angle);
            void *pixels_dst;
            int pitch;
            if (SDL_LockTexture(texture, NULL, &pixels_dst, &pitch) < 0) {
                result = 1;
                goto cleanup;
            }
            memcpy(pixels_dst, pixels, sizeof(pixels));
            SDL_UnlockTexture(texture);

            // Display the texture
            SDL_Rect window_rect = {0, 0, WIDTH, HEIGHT};
            if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0) < 0 ||
                SDL_RenderClear(renderer) < 0 ||
                SDL_RenderCopy(renderer, texture, &window_rect, &window_rect) < 0) {
                result = 1;
                goto cleanup;
            }
            SDL_RenderPresent(renderer);

            // Introduce a delay to control the frame rate
            SDL_Delay(16); // 16 milliseconds (60 frames per second)
        }
    }

cleanup:
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
    return result;
}
