
#include <cstdlib>
#include <ctime>
#include <cstdio>

#include "dset.h"

// Cell Struct stores row and column
struct cell {
	int r, c;

	cell(){ r = 0; c = 0; }
	cell(int r, int c) {this->r = r; this->c = c;}

};

// Swaps cells
void swap(cell &c1, cell &c2) { 
	
	cell temp_cell = c1;
	c1 = c2;
	c2 = temp_cell;

}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Usage: ./Mazemake [row] [col] [output file].\n");
		return 1;
	}

	int Nrow = atoi(argv[1]);
	int Ncol = atoi(argv[2]);
	FILE *file;

	dset ds(Ncol * Nrow);

	// Get total number of walls
	int N = ((Ncol -1) * Nrow) + ((Nrow - 1) * Ncol);

  // Grid wall:     (i0,j0) | (i1,j1)
  // Horizontal wall: (i,j) | (i+1,j)
  // Vertical wall:   (i,j) | (i,j+1)

	cell wall[N][2];

	// Fill wall array
	int k = 0;
	for(int i = 0; i < Ncol - 1; i++){
		for(int j = 0; j < Nrow; j++){
			wall[k][0] = cell(i, j);
			wall[k][1] = cell(i + 1, j);
			k++;
		}
	}

	for(int i = 0; i < Ncol; i++){
		for(int j = 0; j < Nrow - 1; j++){
			wall[k][0] = cell(i, j);
			wall[k][1] = cell(i, j + 1);
			k++;
		}
	}

	// Randomly perturb list order: swap based

	srand(time(NULL));
	for(int i = N - 1; i > 0; i--){
		int j = rand() % (i + 1);
		swap(wall[i][0], wall[j][0]);
		swap(wall[i][1], wall[j][1]);
	}
	// Open output file for writing (argv[3])
	file = fopen(argv[3], "w");
	if(file == NULL){
		printf("Given file could not be open.\n");
		return 0;
	}

	// Write MAZE, Nrow, Ncol header 
	fprintf(file, "MAZE %d %d\n", Nrow, Ncol);

	// Merge disjointed sets and print out walls
	for(k = 0; k < N; k++){

		int ii = wall[k][0].r + (wall[k][0].c * Ncol);
		int jj = wall[k][1].r + (wall[k][1].c * Ncol);

		if(ds.find(ii) != ds.find(jj)){
			ds.merge(ii, jj);
		}else{
			fprintf(file, "%3d %3d %3d %3d\n", wall[k][0].c, wall[k][0].r, wall[k][1].c, wall[k][1].r);
		}

		if(ds.size() == 1){
			break;
		}
	}

	k++;
	// Print out remaining walls
	for(int i = k; i < N; i++){
		fprintf(file, "%3d %3d %3d %3d\n", wall[i][0].c, wall[i][0].r, wall[i][1].c, wall[i][1].r);
	}

	// Close output file

	fclose(file);

	return 0;
}
