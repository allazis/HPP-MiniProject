#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "structs.h"
#include "funcs.h"

typedef struct args {
  cell_t * cells;
  cell_t * cells2;
  int N;
  int n_cells;
  int iCells;
  int totN;
} args_t;

void calc_new_state(int iCells, cell_t * cells, cell_t * cells2, int N, int n_cells, int totN) {
	  /* Copy the part of the data for each cell that is supposed to remain the same. */
  int i, j;
  for(i = iCells; i < iCells + n_cells; i++) {
  	//printf("Start index: %d\n Stop index: %d\n", iCells, iCells + n_cells);
    cells2[i].A_g = cells[i].A_g;
    cells2[i].B_q = cells[i].B_q;
    cells2[i].C_h = cells[i].C_h;
    cells2[i].C_i = cells[i].C_i;
  }

  int ii = 0; 
  int jj = 0;
  /* Find sum of neighbor A-values for each cell. */
  double* neighbor_A_sums = (double*)malloc(n_cells*sizeof(double));
  for(i = iCells; i < iCells + N; i++) {
  	jj = 0;
    for(j = iCells; j < iCells + N; j++) {
      int idx = ii+jj*N;
      /* Find A-values from neighboring cells. */
      double neighbor_A_sum = 0;
      /* neighbor i-1 */
      if(i > 0)
	      neighbor_A_sum += cells[(i-1)+j*totN].A;
      /* neighbor i+1 */
      if(i < totN-1)
	      neighbor_A_sum += cells[(i+1)+j*totN].A;
      /* neighbor j-1 */
      if(j > 0)
	      neighbor_A_sum += cells[i+(j-1)*totN].A;
      /* neighbor j+1 */
      if(j < totN-1)
	      neighbor_A_sum += cells[i+(j+1)*totN].A;
      neighbor_A_sums[idx] = neighbor_A_sum;		// OCH HÄR ÄR FELET OCKSÅ
      jj++;
    }
    ii++;
  }
  //printf("ii: %d\njj: %d\n", ii, jj);

  ii = 0;
  /* Compute new value of A for each cell. */
  for(i = iCells; i < iCells + n_cells; i++) {
    int A_g = cells[i].A_g;
    double A = cells[i].A;
    double B = cells[i].B;
    double C = cells[i].C;
    double neighbor_A_sum = neighbor_A_sums[ii];		// HÄR ÄR FELET hur hämta rätt värde? 
    cells2[i].A = compute_new_A(A_g, A, B, C, neighbor_A_sum);
    ii++;
  }

  free(neighbor_A_sums);

  /* Compute new value of B for each cell. */
  for(i = iCells; i < iCells + n_cells; i++) {
    int B_q = cells[i].B_q;
    double A = cells[i].A;
    double B = cells[i].B;
    double C = cells[i].C;
    cells2[i].B = compute_new_B(B_q, A, B, C);
  }

  /* Compute new value of C for each cell. */
  for(i = iCells; i < iCells + n_cells; i++) {
    int C_h = cells[i].C_h;
    int C_i = cells[i].C_i;
    double A = cells[i].A;
    double B = cells[i].B;
    double C = cells[i].C;
    cells2[i].C = compute_new_C(C_h, C_i, A, B, C);
  }
}

// Function used to parallelise new cell state computation
void * the_thread_func(void* arg) {
  args_t * argPtr = (args_t *)arg;        // Cast input to args struct
  cell_t * cells = argPtr->cells; 
  cell_t * cells2 = argPtr->cells2;          
  int N = argPtr->N;
  int n_cells = argPtr->n_cells;
  int iCells = argPtr->iCells;
  int totN = argPtr->totN;

  calc_new_state(iCells, cells, cells2, N, n_cells, totN);

  return NULL;
 }

int do_one_time_step(int N, cell_t* cells, int n_threads) {

  /* Allocate memory for the result after the timestep. */
  int n_cells = N*N;
  cell_t* cells2 = (cell_t*)malloc(n_cells*sizeof(cell_t));

  /* Declare thread variables */
  args_t thread_args[n_threads];
  int N_thread = (int)(N/n_threads);
  int n_cells_thread = N_thread*N_thread;
  //printf("Number of cells each calc by the %d other thread(s): %d\n", n_threads-1, N_thread);
  pthread_t thread[n_threads];

  /* Start thread. */
  for(int i = 0; i<(n_threads-1);i++){
  	//printf("Hej!\n");
    thread_args[i].cells = cells;
    thread_args[i].cells2 = cells2;
    thread_args[i].N = N_thread;
    thread_args[i].n_cells = n_cells_thread;
    thread_args[i].iCells = i*N_thread;
    thread_args[i].totN = N;
    //printf("Index in cells for thread %d is %d\n", i+1, i*N_thread);
    pthread_create(&thread[i], NULL, the_thread_func, &thread_args[i]);
  }

  int N_main = N - (N_thread*(n_threads-1));
  //printf("Number of cells calc by main: %d\n", N_main);
  int n_cells_main =  N_main*N_main;
  int iCells = (n_threads-1)*N_thread;
  //printf("Index in cells main thread %d is %d\n", n_threads, iCells);
  calc_new_state(iCells, cells, cells2, N_main, n_cells_main, N);

  for(int i = 0; i<(n_threads-1);i++){
    pthread_join(thread[i], NULL);
  }

  /* Copy final result back to cells array. */
  for(int i = 0; i < n_cells; i++) {
    cells[i] = cells2[i];
  }

  /* Free memory. */
  free(cells2);

  /* Return 0 to indicate that the function finished OK. */
  return 0;
}
