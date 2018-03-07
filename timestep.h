#include "structs.h"

int do_one_time_step(int N, cell_t* cells, int n_threads);

void calc_new_state(int iCells, cell_t * cells, cell_t * cells2, int n_cells, int totN);