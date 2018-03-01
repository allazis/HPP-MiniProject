#include <math.h>
#include "utils.h"

double get_sq_contrib(double x, int d) {
  double a = x*x;
  double b = a / d;
  return b;
}

int L = 20;

double B_fun(int B_q) {
  double x = (double)B_q / 1000;
  double sum = 0;
  double product = 1;
  int i;
  for(i = 0; i < L; i++) {
    product *= (x+i*0.007);
    //if(product >= 0)
      sum += product;
  }
  return sum;
}
