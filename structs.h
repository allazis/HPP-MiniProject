#ifndef STRUCTS_HEADER_FILE
#define STRUCTS_HEADER_FILE

struct __attribute__((__packed__)) cell {
  double A;
  double B;
  double C;
  int A_g;
  int B_q;
  int C_h;
  int C_i;
};
typedef struct cell cell_t;

#endif
