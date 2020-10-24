// Compile with g++ -std=c++17 -o osc-sdl osc-sdl.cpp -lSDL2
#include <SDL2/SDL.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

const double radius = 2;
const uint64_t max_iter = 1000;
const int width = 2048;
double z[2][width][width][2] = {0};
std::mutex buffer_mutex[2];

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
    return 1;
  }
  SDL_Window *window =
      SDL_CreateWindow("Oscilloscope", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, width, width, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    fprintf(stderr, "could not create window: %s\n", SDL_GetError());
    return 1;
  }
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  size_t size = 1536, offset = 0;
  std::vector<std::tuple<int, int>> buffer(size);

  SDL_Event event;
  int buffer_id = 0;
  std::chrono::high_resolution_clock::time_point last_fps_t;
  uint64_t fps_counter = 0;
  while (true) {
    SDL_PollEvent(&event);
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
      break;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    uint8_t last_time = 0, last_value = 0;

    for (int i = 0; i < 128; i++) {
      int value, t;
      std::cin >> t >> value;

      int delta_t = (int)t - (int)last_time;
      if (delta_t < 0) {
        delta_t += 128;
      }

      int average_count = 16;
      if (i % average_count == 0) {
        buffer[offset] = {delta_t, value};
      } else {
        std::get<0>(buffer[offset]) += delta_t;
        std::get<1>(buffer[offset]) += value;
        if (i % average_count == average_count - 1) {
          std::get<1>(buffer[offset]) /= average_count;
          offset++;
        }
      }
      if (offset >= size) {
        offset = 0;
      }
    }

    int lastx = 0, lastvalue = 0;
    for (int i = 0; i < size; i++) {
      auto [delta_t, value] = buffer[(offset + i) % size];
      auto x = lastx + ceil(delta_t / 1000.0);
      SDL_RenderDrawLine(renderer, lastx, 7 * (256 - lastvalue), x,
                         7 * (256 - value));
      lastx = x;
      lastvalue = value;
    }

    fps_counter++;
    auto fps_t = std::chrono::high_resolution_clock::now();
    if (fps_t - last_fps_t > std::chrono::seconds(1)) {
      std::stringstream ss;
      ss << int(fps_counter /
                std::chrono::duration<float>(fps_t - last_fps_t).count())
         << " FPS";
      SDL_SetWindowTitle(window, ss.str().c_str());
      last_fps_t = fps_t;
      fps_counter = 0;
    }

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
