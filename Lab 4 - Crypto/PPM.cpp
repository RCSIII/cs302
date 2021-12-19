#include "PPM.h"
#include <fstream>
#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;

bool PPM::read(string file) {
	ifstream ifs;
	string id;
	int imgSize;

	// Open file..
	ifs.open(file.c_str());

	// If file is not open, there is a problem, so return with error
	if (!ifs.is_open()) {
		cerr << "Invalid file given!";
		return false;
	}

	// Dump header info into corresponding variables
	ifs >> id >> rows >> cols >> maxVal;
	// Skip past '\n' after maxVal
	ifs.get();

	// Size of what we are reading in is rows times cols times 3
	imgSize = rows * cols * 3;

	// if maxVal is greater than 255 or less than 0, return with error 
	if (maxVal > 255 || maxVal < 0) {
		cerr << "Invalid Color Intensity given!";
		return false;
	}

	// Create our '2D' img array
	img = new RGB * [rows];
	img[0] = new RGB[rows * cols];

	for (int i = 1; i < get_Nrows(); i++) {
		img[i] = img[i - 1] + get_Ncols();
	}

	// Read in pixels into img array
	ifs.read((char*)img[0], imgSize);
	// Close file...
	ifs.close();

	// If we did not read in all the expected bytes, then there is a problem and we return with error
	if (ifs.gcount() < imgSize) {
		cerr << "Error reading in file! Read in " << ifs.gcount() << " of expected " << imgSize << " number of bytes\n";
		return false;
	}

	// No errors encountered
	return true;
}

bool PPM::write(string file) {
	ofstream ofs;
	// Use substring to remove .ppm from the file name and add our own extension
	string name = file.substr(0, file.length() - 4);
	name = name + "_wmsg.ppm";
	
	// Size of file
	int imgSize = rows * cols * 3;

	// Open the file...
	ofs.open(name.c_str());

	// If file isn't open, there is a problem so return with error
	if (!ofs.is_open()) {
		cerr << "Error creating file!";
		return false;
	}

	// Write header info
	ofs << "P6\n" << get_Nrows() << " " << get_Ncols() << '\n' << maxVal << '\n';

	// Write the pixels to the file
	ofs.write((char*)img[0], imgSize);
	// Close file..
	ofs.close();

	// No errors encountered
	return true;
}