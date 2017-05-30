#ifndef __MAN_THRD_H__
#define __MAN_THRD_H__


#include <thread>

#include "man.h"


class ManThrd : public Man {
public:
  ManThrd(int ww, int wh)
    : Man(ww, wh) { }

private:
};


#endif
