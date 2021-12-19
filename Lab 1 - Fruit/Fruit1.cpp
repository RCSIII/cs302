// Robert Schenck
// Lab 1
// 1/28/2021

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

// fruit structure
struct fruit {
    string name;
    float numPurchased;
    float unitCost;
    bool operator<(const fruit&) const;
      
};

int main(int argc, char* argv[])
{

    // variable declarations
    ifstream file;
    string line;
    string word;
    string mode;
    istringstream iss;
    float fruitCost = 0.0;
    float totalCost = 0.0;
    vector<fruit> fruitVec;
    bool sortall = false;
    bool sortone = false;

    // error checking for proper usage and valid file name
    if (argc < 3) {
        cerr << "Improper usage.\n";
        return 1;
    }
    file.open(argv[2]);
    if (!file.is_open()) {
        cerr << "Invalid file given!\n";
        return 1;
    }

    // gets the given mode from the command line arguments
    mode = argv[1];

    if (mode == "-sortall") {
        sortall = true;
    }
    else if (mode == "-inorder") {
    }
    else if(mode == "-sortone") {
        sortone = true;
    }
    else {
        cerr << "Improoper usage.\n";
    }

    // grabs all the information from the text file
    while (!(file.eof())) {
        fruit fruits;
        getline(file, line);
        iss.str(line);

        // inserts data into struct
        iss >> fruits.name;
        iss >> fruits.numPurchased;
        iss >> fruits.unitCost;

        iss.clear();

        // inserts fruit into vector
        if (fruits.name != "") {
            fruitVec.push_back(fruits);
        }
    }
    file.close();

    // if the sortall option was selected then sort
    if (sortall) {
        stable_sort(fruitVec.begin(), fruitVec.end());
    }
    // if sortone was selected compress the list of fruits
    else if (sortone) {
        stable_sort(fruitVec.begin(), fruitVec.end());
        for (unsigned int i = 0; i < fruitVec.size(); i++) {
            if (i + 1 != fruitVec.size()) {
                if (fruitVec[i].name == fruitVec[i + 1].name) {
                    fruitVec[i].numPurchased += fruitVec[i + 1].numPurchased;
                    fruitVec.erase(fruitVec.begin() + (i+ 1));
                    i--;
                }
            }
        }
    }

    // loop through the vector printing off the values neatly
    for (unsigned int i = 0; i < fruitVec.size(); i++) {
        fruitCost = fruitVec[i].unitCost * fruitVec[i].numPurchased;
        totalCost += fruitCost;
        cout << setw(20) << setfill('.') << left << fruitVec[i].name << " ";
        cout << setw(5) << setfill(' ') << right << fixed << setprecision(2) << fruitVec[i].numPurchased;
        cout << " x " << setw(4) << fixed << fruitVec[i].unitCost;
        cout << " = " << setw(7) << setprecision(2) << fruitCost << "  : ";
        cout << setw(8) << right << setprecision(2) << totalCost << '\n';
    }
}

// compares name of fruits in structs
bool fruit::operator<(const fruit& rhs) const
{
    if(name < rhs.name){
        return true;
    }

    return false;
}
