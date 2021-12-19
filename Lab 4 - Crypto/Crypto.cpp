#include <string>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <algorithm>
#include "PPM.h"
#include "Rnumgen.h"

using namespace std;

const char ETX = 0x3;

// Pixel struct that gives us the rows and cols of a given pixel
struct pixel {
    int row, col;
};

// Sets the pixel that are to be modified for encoding.
void set_pixel_list(PPM& img, vector<pixel>& pixel_list, int seed) {
    pixel p;
    vector<int> histogram(4096);
    int r;

    // Get every pixel into a vector
    for (int i = 0; i < img.get_Nrows(); i ++) {
        for (int j = 0; j < img.get_Ncols(); j ++) {
            p.row = i;
            p.col = j;
            pixel_list.push_back(p);
        }
    }

    // For every pixel...
    for (int i = 0; i < img.get_Nrows(); i++) {
        for (int j = 0; j < img.get_Ncols(); j++) {

            // get the current pixel
            RGB pixel = img[i][j];
            char r, g, b;
            int index;

            // Grab bytes 3-6 for that pixel
            r = (pixel.R >> 3) & 0x1111;
            g = (pixel.G >> 3) & 0x1111;
            b = (pixel.B >> 3) & 0x1111;

            // Create a 12 bit number with that 4 bits, with red being bits 11-8, green being 7-4, and blue being 3-0
            index = (r << 8) | (g << 4) | b;
            // Increment that number inside the histogram
            histogram[index]++;

        }
    }

    // Create a rnumgen object using the user defined seed and the histogram we created
    rnumgen rng(seed, histogram);

    // Create a 24 bit number by calling rand() twice and or them together 
    // Permutate the pixel_list with this 24 bit number
    r = (rng.rand() << 12) | rng.rand();
    for (int i = (int)pixel_list.size() - 1; i > 0; --i) {
        swap(pixel_list[i], pixel_list[r % (i + 1)]);
    }
}

// Encodes a message into a given image at the pixels given to us by pixel_list
void encode(PPM& img, const vector<pixel> pixel_list) {
    int color = 0;
    int pixelListCount = 0;
    char a;

    // While chars are being inputted...
    while (cin.get(a)) {
        // Loop through the 7 bytes of each char, shift the char to the right and clear out all but the LSB, clear the LSB of the pixel we are encoding into
        // Set the LSB of the pixel to be the LSB of the character
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

            // Increment color and pixelListCount to keep track of the pixels and which color we are decoding
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

// Takes an image and a list of pixels and decodes the message hidden in the image
void decode(PPM& img, const vector<pixel> pixel_list) {
    unsigned char pixel;
    int color = 0;
    int pixelListCount = 0;

    while(true){
        // resets the value of 'a'
        char a = '\0';
        // loops through all the 7 byes for each character in the image
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
        // if the current char is not equal to ETX output the char or return, we are done decoding
        if (a != ETX) {
            cout.put(a);
        }
        else {
            return;
        }
    }
}

int main(int argc, char* argv[]) {

    // Command line argument parsing
    if (argc < 3) {
        cerr << "Usage: ./Crypto -[encode|decode] -seed=[number] [PPM file]\n";
        return 1;
    }
    string fname;
    string mode;
    string seedArg;
    int seed = 0;

    if (argc == 3) {
        fname = argv[2];
        mode = argv[1];
    }
    else if (argc == 4) {
        fname = argv[3];
        mode = argv[1];
        seedArg = argv[2];
        if (seedArg.substr(0, 6) != "-seed=") {
            cerr << "Usage: ./Crypto -[encode|decode] -seed=[number] [PPM file]\n";
            return 1;
        }
        if(seedArg.length() < 6) {
            cerr << "Usage: ./Crypto -[encode|decode] -seed=[number] [PPM file]\n";
            return 1;
        }
        else {
            cout << "substring: " << seedArg.substr(6, seedArg.length()) << '\n';
            stringstream str(seedArg.substr(6, seedArg.length()));
            str >> seed;
        }
    }


    // Create image object and create pixel_list vector
    PPM img;
    vector<pixel> pixel_list;

    // Read in image and if there is a problem while reading in exit the program
    if (!img.read(fname)) {
        return 1;
    }
    cout << "seed: " << seed << '\n';
    // Set pixel_list with random pixels
    set_pixel_list(img, pixel_list, seed);

    // If we are encoding, encode a message and then write to a new file
    if (mode == "-encode") {
        encode(img, pixel_list);
        if (!img.write(fname)) {
            return 1;
        }
    }
    // If we are decoding, decode the hidden message;
    else if (mode == "-decode") {
        decode(img, pixel_list);
    }
}