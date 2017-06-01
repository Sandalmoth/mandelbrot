#ifndef __MAN_THRD_H__
#define __MAN_THRD_H__


#include <thread>

#include "man.h"


class ManThrd : public Man {
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
