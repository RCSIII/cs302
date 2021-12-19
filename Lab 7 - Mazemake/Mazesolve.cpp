#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

struct cell {

	int x, y;
	cell() {
		x = 0; y = 0;
	}
	cell(int x, int y) {
		this->x = x;
		this->y = y;
	}

};

int get_dir(int c1x, int c1y, int c2x, int c2y) {

	if (c1x == c2x) {
		if (c1y < c2y) {
			return 2;
		}
		if (c1y > c2y) {
			return 0;
		}
	}
	if (c1y == c2y) {
		if (c1x < c2x) {
			return 3;
		}
		if (c1x > c2x) {
			return 1;
		}
	}

	return 69;
}

bool solve(cell* stack, bool** iswhite, cell source, cell sink, int &stack_size, bool*** wall) {

	iswhite[source.x][source.y] = true;
	stack[stack_size] = source;
	stack_size++;
	cell to;

	if (source.x == sink.x && source.y == sink.y) {
		return true;
	}

	for (int i = 0; i < 4; i++) {
		if (wall[source.x][source.y][i]) {
			continue;
		}

		if (i == 0) {
			to.x = source.x;
			to.y = source.y - 1;
		}
		if (i == 1) {
			to.x = source.x - 1;
			to.y = source.y;
		}
		if (i == 2) {
			to.x = source.x;
			to.y = source.y + 1;
		}
		if (i == 3) {
			to.x = source.x + 1;
			to.y = source.y;
		}

		/*if (to.y > sink.y || to.x > sink.x || to.x < 0 || to.y < 0) {
			continue;
		}*/

		if (iswhite[to.x][to.y]) {
			continue;
		}

		if (solve(stack, iswhite, to, sink, stack_size, wall)) {
			return true;
		}
	}

	iswhite[source.x][source.y] = false;
	stack_size--;

	return false;
}


int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("Usage: ./Mazesolve [input file.txt] [output file.txt]\n");
		return 0;
	}

	// open input file for reading
	// open output file for writing


	FILE* fin;
	FILE* fout;

	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	// detemine Nrow, Ncol from input file

	int Nrow;
	int Ncol;

	fscanf(fin, "MAZE %d %d", &Nrow, &Ncol);
	/* create array of walls for each grid cell
	initialize to have boundary walls set and
	interior walls unset */

	bool ***wall;

	wall = new bool** [Nrow];
	for (int i = 0; i < Nrow; i++) {
		wall[i] = new bool* [Ncol];
		for (int j = 0; j < Ncol; j++) {
			wall[i][j] = new bool[4];
		}
	}

	// Set the boundaries to true
	for (int i = 0; i < Nrow; i++) {
		wall[i][0][0] = true;
		wall[i][Ncol - 1][2] = true;
	}

	for (int i = 0; i < Ncol; i++) {
		wall[0][i][1] = true;
		wall[Nrow - 1][i][3] = true;
	}

	// read input file, set interior walls
	int cx1, cx2, cy1, cy2;

	while (fscanf(fin, "%d %d %d %d", &cx1, &cy1, &cx2, &cy2) == 4) {
		wall[cx1][cy1][get_dir(cx1, cy1, cx2, cy2)] = true;
		wall[cx2][cy2][get_dir(cx2, cy2, cx1, cy1)] = true;
	}

	// initalize DFS path computation

	cell* stack = new cell[Nrow * Ncol];
	int stack_size = 0;
	bool** iswhite;
	iswhite = new bool* [Nrow];
	for (int i = 0; i < Nrow; i++) {
		iswhite[i] = new bool[Ncol];
	}
	cell source;
	cell sink(Nrow - 1, Ncol - 1);

	// carry out DFS source-to-sink computation
	printf("%d\n", solve(stack, iswhite, source, sink, stack_size, wall));
	// write PATH, Nrow, Ncol header
	fprintf(fout, "PATH %d %d\n", Nrow, Ncol);

	// write cells on path to file
	for (int i = 0; i < stack_size; i++) {
		fprintf(fout, "%d %d\n", stack[i].x, stack[i].y);
	}

	// close open files and delete allocated mem
	for (int i = 0; i < Nrow; i++) {
		for (int j = 0; j < Ncol; j++) {
			delete[] wall[i][j];
		}
		delete[] wall[i];
		delete[] iswhite[i];
	}
	delete[] wall;
	delete[] iswhite;
	delete[] stack;

	fclose(fin);
	fclose(fout);
}