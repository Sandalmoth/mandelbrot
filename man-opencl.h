#ifndef __MAN_OPENCL_H__
#define __MAN_OPENCL_H__


#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include "man.h"


class ManOpenCL : public Man {
public:
  ManOpenCL();

  void update() override;
private:
  cl::Context context;
  cl::Device device;
  cl::Program program;
  cl::Kernel kernel;
};


#endif
