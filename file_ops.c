#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_ops.h"

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

int write_output_file(const char* output_file_name,
		      int n_cells,
		      const cell_t* cells) {
  FILE* f = fopen(output_file_name, "wb");
  if(f == NULL) {
    printf("ERROR in write_output_file: failed to open file '%s' for writing.\n",
	   output_file_name);
    return -1;
  }
  size_t size_for_each_cell = 3*sizeof(double) + 4*sizeof(int);
  size_t buf_size = n_cells*size_for_each_cell;
  char* buf = (char*)malloc(buf_size*sizeof(char));
  /* Prepare data for one cell at a time. */
  char* p = buf;
  int i;
  for(i = 0; i < n_cells; i++) {
    const cell_t* cellPtr = &cells[i];
    /* Ag */
    memcpy(p, &cellPtr->A_g, sizeof(int));
    p += sizeof(int);
    /* A */
    memcpy(p, &cellPtr->A, sizeof(double));
    p += sizeof(double);
    /* B_q */
    memcpy(p, &cellPtr->B_q, sizeof(int));
    p += sizeof(int);
    /* B */
    memcpy(p, &cellPtr->B, sizeof(double));
    p += sizeof(double);
    /* C_h */
    memcpy(p, &cellPtr->C_h, sizeof(int));
    p += sizeof(int);
    /* C */
    memcpy(p, &cellPtr->C, sizeof(double));
    p += sizeof(double);
    /* C_i */
    memcpy(p, &cellPtr->C_i, sizeof(int));
    p += sizeof(int);
  }
  size_t n_written = fwrite(buf, sizeof(char), buf_size, f);
  if(n_written != buf_size) {
    printf("Error in write_output_file: failed to write to file '%s'.\n",
	   output_file_name);
    return -1;
  }
  fclose(f);
  free(buf);
  return 0;
}
