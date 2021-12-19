#include <string>
#include <iostream>
#include <vector>
#include <string.h>
#include "PPM.h"
#include "Rnumgen.h"

using namespace std;

const char ETX = 0x3;

// Pixel struct that gives us the rows and cols of a given pixel
struct pixel {
    int row, col;
};

// Sets the pixel that are to be modified for encoding, in this case, every other pixel
void set_pixel_list(PPM& img, vector<pixel>& pixel_list) {
    pixel p;
    for (int i = 0; i < img.get_Nrows(); i += 2) {
        ;
        for (int j = 0; j < img.get_Ncols(); j += 2) {
            p.row = i;
            p.col = j;
            pixel_list.push_back(p);
        }
    }
}

// Encodes a message into a given image at the pixels given to us by pixel_list
void encode(PPM& img, const vector<pixel> pixel_list) {
    int color = 0;
    int pixelListCount = 0;
    char a;

	// While chars are being inputed
    while (cin.get(a)) {
		// Loop through the 7 bytes of each char, shift the char to the right and clear out all but the LSB, clear the LSB of the pixel we are encoding into
		// // Set the LSB oft eh pixel to be the LSB of the character
        for (int k = 0; k < 7; k++) {

            if (color % 3 == 0) {
                img[pixel_list[pixelListCount].row][pixel_list[pixelListCount].col].R &= 0xfe;
                img[pixel_list[pixelListCount].row][pixel_list[pixelListCount].col].R |= ((a >> k) & 0x1);
            }
            else if (color % 3 == 1) {
                img[pixel_list[pixelListCount].row][pixel_list[pixelListCount].col].G &= 0xfe;
                img[pixel_list[pixelListCount].row][pixel_list[pixelListCount].col].G |= ((a >> k) & 0x1);
            }
            else if (color % 3 == 2) {
                img[pixel_list[pixelListCount].row][pixel_list[pixelListCount].col].B &= 0xfe;
                img[pixel_list[pixelListCount].row][pixel_list[pixelListCount].col].B |= ((a >> k) & 0x1);
            }
			// Increment color and pixelListCount to keep track of the pixels and which color we are encoding
            color++;
            pixelListCount++;
        }
    }

	// Now that we are done getting pixels do the same thing as above but with the ETX character
    a = ETX;

    for (int k = 0; k < 7; k++) {

        if (color % 3 == 0) {
            img[pixel_list[pixelListCount].row][pixel_list[pixelListCount].col].R &= 0xfe;
            img[pixel_list[pixelListCount].row][pixel_list[pixelListCount].col].R |= ((a >> k) & 0x1);
        }
        else if (color % 3 == 1) {
            img[pixel_list[pixelListCount].row][pixel_list[pixelListCount].col].G &= 0xfe;
            img[pixel_list[pixelListCount].row][pixel_list[pixelListCount].col].G |= ((a >> k) & 0x1);
        }
        else if (color % 3 == 2) {
            img[pixel_list[pixelListCount].row][pixel_list[pixelListCount].col].B &= 0xfe;
            img[pixel_list[pixelListCount].row][pixel_list[pixelListCount].col].B |= ((a >> k) & 0x1);
        }
        color++;
        pixelListCount++;
    }

}

// Take an image and a list of pixels and decodes the message hidden in the image
void decode(PPM& img, const vector<pixel> pixel_list) {
    unsigned char pixel;
    int color = 0;
    int pixelListCount = 0;

    while(true){
		// resets of the value of 'a'
        char a = '\0';
		// Loops through all the 7 bytes for each character in the image
		// undo what was done in encode by clearing all but the LSB and shifting right k times
        for (int k = 0; k < 7; k++) {
            if (color % 3 == 0) {
                pixel = img[pixel_list[pixelListCount].row][pixel_list[pixelListCount].col].R;
            }
            else if (color % 3 == 1) {
                pixel = img[pixel_list[pixelListCount].row][pixel_list[pixelListCount].col].G;
            }
            else if (color % 3 == 2) {
                pixel = img[pixel_list[pixelListCount].row][pixel_list[pixelListCount].col].B;
            }
            pixel &= 0x1;
            pixel <<= k;

            a |= pixel;

			// Increment color and pixelListCount to keep track of the pixels and which color we are decoding
            color++;
            pixelListCount++;
        }
		// If the current char is not equal to ETX, output the char or return, we are done encoding
        if (a != ETX) {
            cout.put(a);
        }
        else {
            return;
        }
    }
}

int main(int argc, char* argv[]) {

	// Command line arugment parsing
    if (argc < 3 || ((strcmp(argv[1], "-encode") && strcmp(argv[1], "-decode")))) {
        cerr << "Usage: ./Crypto -[encode|decode] [PPM file]\n";
        return 1;
    }

	// Create an PPM object
    PPM img;
    string fname = argv[2];
    string mode = argv[1];
    vector<pixel> pixel_list;

	// Read in image
    if (!img.read(fname)) {
        return 1;
    }
	// Set list of target pixels
    set_pixel_list(img, pixel_list);

	// If we are encoding, encode a message and then write to a new file
    if (mode == "-encode") {
        encode(img, pixel_list);
        if (!img.write(fname)) {
            return 1;
        }
    }
	// If we are decoding, decode the hidden message
    else if (mode == "-decode") {
        decode(img, pixel_list);
    }
}
