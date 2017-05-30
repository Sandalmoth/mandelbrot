#ifndef __MAN_H__
#define __MAN_H__


#include <vector>
#include <cstdint>
#include <cmath>
#include <iostream>


const double LOG2 = log(2);
const double WMAX = 3.5;
const double HMAX = 2.5;
const double ZOOM = 0.9;

class Man {
public:
  void set_dims(int ww, int wh) {
    window_width = ww;
    window_height = wh;
  }

  void set_target(double _x, double _y) {
    wx = _x;
    wy = _y;
  }

  void set_zoom(double _w, double _h) {
    w = _w;
    h = _h;
  }

  void set_max_iters(uint64_t mx) {
    max_iters = mx;
  }

  void set_radius(int r) {
    radius = r;
  }

  void zoom_towards(double x, double y) {
    w *= ZOOM;
    h *= ZOOM;
    double mxx = wx - w/2.0 + w*x;
    double myy = wy - h/2.0 + h*y;
    wx = (wx + mxx) / 2.0;
    wy = (wy + myy) / 2.0;
    std::cout << w << ' ' << h << ' ' << wx << ' ' << wy << std::endl;
    window_bounds();
  }

  void zoom_away() {
    w /= ZOOM;
    h /= ZOOM;
    if (w > WMAX) w = WMAX;
    if (h > HMAX) h = HMAX;
    window_bounds();
  }

  void window_bounds() {
    if (wx - w < -WMAX / 2.0) {
      wx += -WMAX / 2.0 - (wx - w);
    }
    if (wy - h < -HMAX / 2.0) {
      wy += -HMAX / 2.0 - (wy - h);
    }
    if (wx + w > WMAX / 2.0) {
      wx += WMAX / 2.0 - (wx + w);
    }
    if (wy + h > HMAX / 2.0) {
      wy += HMAX / 2.0 - (wy + h);
    }
  }

  const std::vector<double> &get_data() const {
    return data;
  };

  virtual void update() {
    data = std::vector<double>(window_width*window_height, 0);
    for (int px = 0; px < window_width; ++px) {
      for (int py = 0; py < window_height; ++py) {
        double x0 = wx - w/2.0 + px*w/static_cast<double>(window_width);
        double y0 = wy - h/2.0 + py*h/static_cast<double>(window_height);
        double x = 0.0;
        double y = 0.0;
        uint64_t i = 0;
        while (x*x + y*y < radius && i < max_iters) {
          double xt = x*x - y*y + x0;
          y = 2*x*y + y0;
          x = xt;
          ++i;
        }
        double i_adj = i;
        if (i < max_iters) {
          double log_zn = log(x*x + y*y) / 2;
          double nu = log(log_zn / LOG2) / LOG2;
          i_adj = static_cast<double>(i) + 1.0 - nu;
        }
        data[py * window_width + px] = i_adj;
      }
    }
  }

private:
  std::vector<double> data;
  double wx = -0.75;
  double wy = 0.0;
  double w = 3.5;
  double h = 2.5;
  int window_width = 1536;
  int window_height = 1024;
  uint64_t max_iters = 1000;
  double radius = 1e100;
};


#endif
