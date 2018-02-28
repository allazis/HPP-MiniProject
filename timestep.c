#include <stdlib.h>
#include <stdio.h>
#include "structs.h"
#include "funcs.h"

int do_one_time_step(int N, cell_t* cells) {

  /* Allocate memory for the result after the timestep. */
  int n_cells = N*N;
  cell_t* cells2 = (cell_t*)malloc(n_cells*sizeof(cell_t));

  /* Copy the part of the data for each cell that is supposed to remain the same. */
  int i, j;
  for(i = 0; i < n_cells; i++) {
    cells2[i].A_g = cells[i].A_g;
    cells2[i].B_q = cells[i].B_q;
    cells2[i].C_h = cells[i].C_h;
    cells2[i].C_i = cells[i].C_i;
  }

  /* Find sum of neighbor A-values for each cell. */
  double* neighbor_A_sums = (double*)malloc(n_cells*sizeof(double));
  for(i = 0; i < N; i++) {
    for(j = 0; j < N; j++) {
      int idx = i+j*N;
      /* Find A-values from neighboring cells. */
      double neighbor_A_sum = 0;
      /* neighbor i-1 */
      if(i > 0)
	neighbor_A_sum += cells[(i-1)+j*N].A;
      /* neighbor i+1 */
      if(i < N-1)
	neighbor_A_sum += cells[(i+1)+j*N].A;
      /* neighbor j-1 */
      if(j > 0)
	neighbor_A_sum += cells[i+(j-1)*N].A;
      /* neighbor j+1 */
      if(j < N-1)
	neighbor_A_sum += cells[i+(j+1)*N].A;
      neighbor_A_sums[idx] = neighbor_A_sum;
    }
  }

  /* Compute new value of A for each cell. */
  for(i = 0; i < n_cells; i++) {
    int A_g = cells[i].A_g;
    double A = cells[i].A;
    double B = cells[i].B;
    double C = cells[i].C;
    double neighbor_A_sum = neighbor_A_sums[i];
    cells2[i].A = compute_new_A(A_g, A, B, C, neighbor_A_sum);
  }

  /* Compute new value of B for each cell. */
  for(i = 0; i < n_cells; i++) {
    int B_q = cells[i].B_q;
    double A = cells[i].A;
    double B = cells[i].B;
    double C = cells[i].C;
    cells2[i].B = compute_new_B(B_q, A, B, C);
  }

  /* Compute new value of C for each cell. */
  for(i = 0; i < n_cells; i++) {
    int C_h = cells[i].C_h;
    int C_i = cells[i].C_i;
    double A = cells[i].A;
    double B = cells[i].B;
    double C = cells[i].C;
    cells2[i].C = compute_new_C(C_h, C_i, A, B, C);
  }

  /* Copy final result back to cells array. */
  for(i = 0; i < n_cells; i++) {
    cells[i] = cells2[i];
  }

  /* Free memory. */
  free(cells2);
  free(neighbor_A_sums);

  /* Return 0 to indicate that the function finished OK. */
  return 0;
}
