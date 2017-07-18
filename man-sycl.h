#ifndef __MAN_SYCL_H__
#define __MAN_SYCL_H__


#include <thread>
#include <CL/sycl.hpp>

#include "man.h"


class ManSYCL : public Man {
public:
  void update() override;

private:
};


#endif
