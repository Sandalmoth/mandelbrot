#include "man-thrd.h"


#include <list>


using namespace std;


void ManThrd::update() {
  data = vector<double>(window_width*window_height, 0);
  int tuse = n_threads;
  if (tuse > window_width) tuse = window_width;
  list<thread> threads;
  for (int i = 0; i < tuse; ++i) {
    int bonus = window_width % tuse;
    if (i == 0)
      threads.emplace_back(run_segment(window_width / tuse * i, 0, window_width/tuse + bonus, window_height));
    else
      threads.emplace_back(run_segment(window_width / tuse * i + bonus, 0, window_width/tuse, window_height));
  }
  for (auto &t: threads) {
    t.join();
  }
}


double ManThrd::mandelpoint(int px, int py) {
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
  return i_adj;
}


void ManThrd::_run_segment(int x_pos, int y_pos, int width, int height) {
  cout << "Segment " << x_pos << ' ' << y_pos << ' ' << width << ' ' << height << endl;
  for (int px = x_pos; px < x_pos + width; ++px) {
    for (int py = y_pos; py < y_pos + height; ++py) {
      data[py * window_width + px] = mandelpoint(px, py);
    }
  }
}
