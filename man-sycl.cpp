#include "man-sycl.h"


#include <iostream>
#include <CL/sycl.hpp>


using namespace cl::sycl;


void ManSYCL::update() {
  std::cout << "Sadly, SPIR 1.2 doesn't seem to run on the open source amdgpu driver. So since I can't run it on any of my machines anyway, it's gonna remain unimplemented." << std::endl;
}
