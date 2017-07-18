#include "man-opencl.h"


#include <fstream>


ManOpenCL::ManOpenCL() {
  try {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    std::cerr << "Platform number is: " << platforms.size() << std::endl;

    if (platforms.size() == 0) {
      std::cerr << "No OpenCL platforms found. Aborting" << std::endl;
      exit(1);
    }

    std::string vendor;
    platforms[0].getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &vendor);
    std::cerr << "Platform is by: " << vendor << "\n";

    // Make context
    cl_context_properties cprops[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(), 0};
    context = cl::Context(CL_DEVICE_TYPE_GPU, cprops);

    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
    device = devices[0];

    // make kernel
    std::ifstream source_file("mpt.cl");
    std::string source_code(  std::istreambuf_iterator<char>(source_file)
                            , (std::istreambuf_iterator<char>()));
    cl::Program::Sources source(1, std::make_pair(source_code.c_str(), source_code.length()+1));

    program = cl::Program(context, source);
    try {
      program.build(devices);
    } catch (...) {
      // Print build info for all devices
      cl_int buildErr = CL_SUCCESS;
      auto buildInfo = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device, &buildErr);
      std::cerr << buildInfo << std::endl << std::endl;
      exit(1);
    }

    // Make kernel
  kernel = cl::Kernel(program, "mandelpoint");

  } catch (cl::Error e) {
    std::cout << "OpenCL error\n  " << e.what() << "(" << e.err() << ")" << std::endl;
    exit(1);
  }
  std::cout << "update end" << std::endl;
}


void ManOpenCL::update() {
}
