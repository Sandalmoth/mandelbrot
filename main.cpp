#include <iostream>
#include <map>
#include <memory>
#include <SDL2/SDL.h>
#include <string>
#include <tclap/CmdLine.h>
#include <tuple>

#include "man.h"
#include "man-thrd.h"


using namespace std;


const string VERSION = "0.2";


struct Parameters {
  int window_width;
  int window_height;
  uint64_t max_iters;
};


struct State {
  int mouse_x;
  int mouse_y;
  int mip_width;
  int mip_height;
  int mip = 1;
  int mipdiv = 1 << (1 - mip);
};


tuple<int, int, int> interpolate_colour(tuple<int, int, int > a, tuple<int, int, int> b, double x);


int main(int argc, char **argv) {
  Parameters p;
  try {
    TCLAP::CmdLine cmd("Mandelbrot drawer", ' ', VERSION);
    TCLAP::ValueArg<int> a_window_width("", "window-width", "Width of window in pixels", false, 768, "positive integer", cmd);
    TCLAP::ValueArg<int> a_window_height("", "window-height", "Height of window in pixels", false, 512, "positive integer", cmd);
    TCLAP::ValueArg<int> a_max_iters("", "max-iters", "Maximum iterations per pixel default", false, 1000, "positive integer", cmd);

    cmd.parse(argc, argv);

    p.window_width = a_window_width.getValue();
    p.window_height = a_window_height.getValue();
    p.max_iters = a_max_iters.getValue();
  } catch (TCLAP::ArgException &e) {
    cerr << "TCLAP Error: " << e.error() << endl << "\targ: " << e.argId() << endl;
  }

  unique_ptr<Man> mandel;
  // TODO add selection code
  // mandel = make_unique<Man>();
  mandel = make_unique<ManThrd>();
  mandel->set_n_threads(5);

  mandel->set_dims(p.window_width, p.window_height);
  mandel->set_max_iters(p.max_iters);

  // Set up SDL
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow
    ( "Mandelbrot"
      , SDL_WINDOWPOS_UNDEFINED
      , SDL_WINDOWPOS_UNDEFINED
      , p.window_width
      , p.window_height
      , SDL_WINDOW_SHOWN
      );
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // Generate palette
  vector<tuple<int, int, int>> palette;
  palette.push_back(tuple<int, int, int>{0xAF, 0xF, 0xF});
  palette.push_back(tuple<int, int, int>{0x0, 0xF, 0x40});
  palette.push_back(tuple<int, int, int>{0xAE, 0x0, 0x78});

  // main loop
  SDL_Event event;
  bool run = true;
  bool redraw = true;
  bool update = true;
  State s;
  s.mip_width = p.window_width;
  s.mip_height = p.window_height;
  while (run) {

    if (update) {
      s.mipdiv = 1 << (s.mip - 1);
      s.mip_width = p.window_width/s.mipdiv;
      s.mip_height = p.window_height/s.mipdiv;
      mandel->set_dims(s.mip_width, s.mip_height);
      mandel->update();
      redraw = true;
      update = false;
      if (s.mip > 1) {
        --s.mip;
        update = true;
      }
    }

    if (redraw) {
      SDL_SetRenderDrawColor( renderer, 0x0, 0x0, 0x0, 0xFF);
      SDL_RenderClear(renderer);

      auto data = mandel->get_data();
      for (int x = 0; x < s.mip_width; ++x) {
        for (int y = 0; y < s.mip_height; ++y) {
          double i = data[y*s.mip_width + x];
          auto colour1 = palette[static_cast<int>(i) % palette.size()];
          auto colour2 = palette[static_cast<int>(i+1.0) % palette.size()];
          auto icol = interpolate_colour(colour1, colour2, i - floor(i));
          if (i >= static_cast<double>(p.max_iters)) {
            SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
          } else {
            SDL_SetRenderDrawColor(renderer, get<0>(icol), get<1>(icol), get<2>(icol), 0xFF);
          }
          if (s.mip > 0) {
            SDL_Rect rect = {x*s.mipdiv, y*s.mipdiv, s.mipdiv, s.mipdiv};
            SDL_RenderFillRect(renderer, &rect);
          } else {
            SDL_RenderDrawPoint(renderer, x, y);
          }
        }
      }

      SDL_RenderPresent(renderer);
      redraw = false;
    }

    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        run = false;
      } else if (event.type == SDL_MOUSEWHEEL) {
        if (event.wheel.y > 0) {
          mandel->zoom_towards(s.mouse_x / static_cast<double>(p.window_width), s.mouse_y / static_cast<double>(p.window_height));
        } else if (event.wheel.y < 0) {
          mandel->zoom_away();
        }
        if (s.mip < 8) {
          ++s.mip;
        }
        update = true;
      } else if (event.type == SDL_MOUSEMOTION) {
        s.mouse_x = event.motion.x;
        s.mouse_y = event.motion.y;
      }
    }
  }

  // Exit SDL
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}


tuple<int, int, int> interpolate_colour(tuple<int, int, int> a, tuple<int, int, int> b, double i) {
  double x, y, z;
  x = get<0>(a) + (get<0>(b) - get<0>(a)) * i;
  y = get<1>(a) + (get<1>(b) - get<1>(a)) * i;
  z = get<2>(a) + (get<2>(b) - get<2>(a)) * i;
  return tuple<int, int, int>{x, y, z};
}
