#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "funcs.h"
#include "utils.h"

double compute_new_A(int A_g,
		     double A,
		     double B,
		     double C,
		     double neighbor_A_sum) {
  const int nnn = 5;
  double* tmpBuf = (double*)malloc(nnn*sizeof(double));
  tmpBuf[0] = 0.001*B;
  tmpBuf[1] = 0.002*C;
  tmpBuf[2] = neighbor_A_sum * 0.005;
  tmpBuf[3] = 1e-6*sqrt(A*A+1);
  tmpBuf[4] = sqrt(A*A+2);
  double sum = 0;
  int i;
  for(i = 0; i < nnn; i++)
    sum += tmpBuf[i];
  if(A_g) {
    double extraSum = A*A/(1+B);
    if(extraSum > 5)
      extraSum = 5;
    sum += extraSum;
  }
  if(A > 1)
    sum = sum / A;
  free(tmpBuf);
  return A + sum;
}

double compute_new_B(int B_q,
		     double A,
		     double B,
		     double C) {
  double increase = B_fun(B_q) + 0.01*A + 0.001*C;
  if(B > 1)
    increase = increase / B;
  return B + increase;
}

double compute_new_C(int C_h,
		     int C_i,
		     double A,
		     double B,
		     double C) {
  int tmp1a = C_h / (C_i + 1);
  int tmp1b = C_h / (C_i + 2);
  double tmp2 = tmp1a + tmp1b;
  tmp2 *= 0.0001;
  double tmp3 = (A + B) / 800;
  double tmp4 = get_sq_contrib(C, C_i+2) / 9000;
  double increase = tmp2 + tmp3 + tmp4;
  if(C > 1)
    increase = increase / (C*C);
  return C + increase;
}
