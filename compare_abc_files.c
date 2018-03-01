#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct cell {
  int A_g;
  double A;
  int B_q;
  double B;
  int C_h;
  double C;
  int C_i;
};
typedef struct cell cell_t;

int read_input_file(const char* input_file_name,
		    int n_cells,
		    cell_t* cells) {
  FILE* f = fopen(input_file_name, "rb");
  if(f == NULL) {
    printf("ERROR in read_input_file: failed to open file '%s'\n",
	   input_file_name);
    return -1;
  }
  size_t size_for_each_cell = 3*sizeof(double) + 4*sizeof(int);
  size_t buf_size = n_cells*size_for_each_cell;
  /* Get filesize using fseek() and ftell(). */
  fseek(f, 0L, SEEK_END);
  size_t fileSize = ftell(f);
  /* Now use fseek() again to set file position back to beginning of the file. */
  fseek(f, 0L, SEEK_SET);
  if(fileSize != buf_size) {
    printf("read_input_file error: size of input file '%s' does not match the given problem size.\n", input_file_name);
    return -1;
  }
  char* buf = (char*)malloc(buf_size*sizeof(char));
  size_t n_read = fread(buf, sizeof(char), buf_size, f);
  if(n_read != buf_size) {
    printf("Error in read_input_file: failed to read file '%s'.\n",
	   input_file_name);
    return -1;
  }
  fclose(f);
  /* Get data for one cell at a time. */
  const char* p = buf;
  int i;
  for(i = 0; i < n_cells; i++) {
    cell_t* cellPtr = &cells[i];
    memset(cellPtr, 0x00, sizeof(cell_t));
    /* Ag */
    memcpy(&cellPtr->A_g, p, sizeof(int));
    p += sizeof(int);
    /* A */
    memcpy(&cellPtr->A, p, sizeof(double));
    p += sizeof(double);
    /* B_q */
    memcpy(&cellPtr->B_q, p, sizeof(int));
    p += sizeof(int);
    /* B */
    memcpy(&cellPtr->B, p, sizeof(double));
    p += sizeof(double);
    /* C_h */
    memcpy(&cellPtr->C_h, p, sizeof(int));
    p += sizeof(int);
    /* C */
    memcpy(&cellPtr->C, p, sizeof(double));
    p += sizeof(double);
    /* C_i */
    memcpy(&cellPtr->C_i, p, sizeof(int));
    p += sizeof(int);
  }
  free(buf);
  return 0;
}

static void update_maxdiff(double x1, double x2, double* maxabsdiff) {
  double diff = x1 - x2;
  double absdiff = fabs(diff);
  if(absdiff > *maxabsdiff)
    *maxabsdiff = absdiff;
}

/* The idea with the check_that_number_seem_OK() function is to check
   that there are no strange numbers like "nan" that may give problems
   when we try to compare the numbers later. */
int check_that_number_seem_OK(double x) {
  const double minAllowedValue = -1e20;
  const double maxAllowedValue = 1e20;
  double a = x;
  if(a >= minAllowedValue && a <= maxAllowedValue)
    return 0;
  else
    return -1;
}

static int check_cell(cell_t cell) {
  if(check_that_number_seem_OK(cell.A) != 0) {
    printf("Error: bad A value found: %lf\n", cell.A);
    return -1;
  }
  if(check_that_number_seem_OK(cell.B) != 0) {
    printf("Error: bad B value found: %lf\n", cell.B);
    return -1;
  }
  if(check_that_number_seem_OK(cell.C) != 0) {
    printf("Error: bad C value found: %lf\n", cell.C);
    return -1;
  }
  return 0;
}


int main(int argc, const char* argv[]) {
  if(argc != 4) {
    printf("Give 3 input args: N file1.abc file2.abc\n");
    return -1;
  }
  int N = atoi(argv[1]);
  const char* fileName1 = argv[2];
  const char* fileName2 = argv[3];
  printf("N = %d\n", N);
  printf("fileName1 = '%s'\n", fileName1);
  printf("fileName2 = '%s'\n", fileName2);

  /* Allocate memory. */
  int n_cells = N*N;
  cell_t* cells1 = (cell_t*)malloc(n_cells*sizeof(cell_t));
  cell_t* cells2 = (cell_t*)malloc(n_cells*sizeof(cell_t));

  /* Read input file 1. */
  if(read_input_file(fileName1, n_cells, cells1) != 0) {
    printf("Error: failed to read input file 1.\n");
    return -1;
  }

  /* Read input file 2. */
  if(read_input_file(fileName2, n_cells, cells2) != 0) {
    printf("Error: failed to read input file 2.\n");
    return -1;
  }

  /* Check file contents. */
  double maxabsdiff = 0;
  int i;
  for(i = 0; i < n_cells; i++) {
    if(check_cell(cells1[i]) != 0) {
      printf("Error: file 1 contains bad data.\n");
      return -1;
    }
    if(check_cell(cells2[i]) != 0) {
      printf("Error: file 2 contains bad data.\n");
      return -1;
    }
    if(cells1[i].A_g != cells2[i].A_g) {
      printf("Error: A_g values do not match.\n");
      return -1;
    }
    if(cells1[i].B_q != cells2[i].B_q) {
      printf("Error: B_q values do not match.\n");
      return -1;
    }
    if(cells1[i].C_h != cells2[i].C_h) {
      printf("Error: C_h values do not match.\n");
      return -1;
    }
    if(cells1[i].C_i != cells2[i].C_i) {
      printf("Error: C_i values do not match.\n");
      return -1;
    }
    update_maxdiff(cells1[i].A, cells2[i].A, &maxabsdiff);
    update_maxdiff(cells1[i].B, cells2[i].B, &maxabsdiff);
    update_maxdiff(cells1[i].C, cells2[i].C, &maxabsdiff);
  }
  printf("maxabsdiff  =  %17.14f  =  %7.3g\n", maxabsdiff, maxabsdiff);
  return 0;
}
