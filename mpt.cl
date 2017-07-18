#define LOG2 0.693148

__kernel void mpt(__global const float *k, __global float *out) {
  int i = get_global_id(0);
  out[i] = k[i];
};

/* __kernel void mpt( */
/*                     const __global int *pw */
/*                   , const __global int *ph */
/*                   , const __global double w */
/*                   , const __global double h */
/*                   , const __global double window_width; */
/*                   , const __global double window_height; */
/*                   , const __global double radius; */
/*                   , const __global int max_iters; */
/*                   , __global double *out */
/*                   ) { */
/*   int id = get_global_id(0); */
/*   double x0 = wx - w/2.0 + px[i]*w/window_width; */
/*   double y0 = wy - h/2.0 + py[i]*h/window_height; */
/*   double x = 0.0; */
/*   double y = 0.0; */
/*   int i = 0; */
/*   while (x*x + y*y < radius && i < max_iters) { */
/*     double xt = x*x - y*y + x0; */
/*     y = 2*x*y + y0; */
/*     x = xt; */
/*     ++i; */
/*   } */
/*   double i_adj = i; */
/*   if (i < max_iters) { */
/*     double log_zn = log(x*x + y*y) / 2; */
/*     double nu = log(log_zn / LOG2) / LOG2; */
/*     i_adj = (double)i + 1.0 - nu; */
/*   } */
/*   out[i] = i_adj; */
/* } */
