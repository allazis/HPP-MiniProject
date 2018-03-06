#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "file_ops.h"
#include "timestep.h"

int main(int argc, char* argv[]) {
  /* Get input arguments. */
  if(argc != 5) {
    printf("Please give 4 input arguments: N n_steps input_file_name n_threads\n");
    return -1;
  }
  int N = atoi(argv[1]);
  int n_steps = atoi(argv[2]);
  const char* input_file_name = argv[3];
  int n_threads = atoi(argv[4]);

  /* Print input arguments. */
  printf("Input arguments:\n");
  printf("N = %d\n", N);
  printf("n_steps = %d\n", n_steps);
  printf("input_file_name = %s\n", input_file_name);
  printf("n_threads = %d\n", n_threads);

  /* Allocate memory. */
  int n_cells = N*N;
  cell_t* cells = (cell_t*)malloc(n_cells*sizeof(cell_t));

  /* Read input file. */
  if(read_input_file(input_file_name, n_cells, cells) != 0) {
    printf("Error: failed to read input file.\n");
    return -1;
  }

  /* Do simulation for the given number of timesteps. */
  printf("Starting simulation...\n");
  int i;
  for(i = 0; i < n_steps; i++) {
    if(do_one_time_step(N, cells, n_threads) != 0) {
      printf("Error: do_one_time_step failed.\n");
      return -1;
    }
  }
  printf("Simulation done, now writing result file...\n");

  /* Write result file. */
  const char* result_file_name = "result.abc";
  if(write_output_file(result_file_name, n_cells, cells) != 0) {
    printf("Error: failed to write result file.\n");
    return -1;
  }
  printf("Done. Result file '%s' created.\n", result_file_name);

  /* Free memory. */
  free(cells);

  /* Return 0 to indicate that the program finished OK. */
  return 0;
}
