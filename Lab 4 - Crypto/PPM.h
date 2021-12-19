#ifndef __PPM_H__
#define __PPM_H__

#include <string>

using namespace std;

struct RGB {
	RGB() { R = 0; G = 0; B = 0; }

	unsigned char R, G, B;
};

class PPM {
public:
	PPM() { rows = 0; cols = 0; maxVal = 0; };
	// Deallocates memory
	~PPM() { delete img; }

	bool read(string);
	bool write(string);

	// Allows for 2D array like access to pixels
	RGB* operator[](int i) { return img[i]; }

	// Returns dimensions of image
	int get_Nrows() { return rows; }
	int get_Ncols() { return cols; }

private:
	int rows, cols, maxVal;
	RGB** img;
};

#endif
