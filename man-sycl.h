#ifndef __MAN_SYCL_H__
#define __MAN_SYCL_H__


#include <thread>
#include <CL/sycl.hpp>

#include "man.h"


class ManSYCL : public Man {
public:
  void update() override;

private:
  double mandelpoint(int px, int py);
  void _run_segment(int x_pos, int y_pos, int width, int height);
  std::thread run_segment(int x_pos, int y_pos, int width, int height) {
    return std::thread([=]{ _run_segment(x_pos, y_pos, width, height); });
  }

};


#endif
