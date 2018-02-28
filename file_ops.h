#include "structs.h"

int read_input_file(const char* input_file_name,
		    int n_cells,
		    cell_t* cells);

int write_output_file(const char* output_file_name,
		      int n_cells,
		      const cell_t* cells);
