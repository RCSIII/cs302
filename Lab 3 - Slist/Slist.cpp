// Robert Schenck
// Lab 3

#include "Slist.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <string.h>

using namespace std;

class Data {
public:

    // overload << and >> for input and output
    friend istream& operator>>(istream&, Data&);
    friend ostream& operator<<(ostream&, const Data&);

    // overload < to compare info
    bool operator<(const Data&) const;
    // count helps keep track of amount of din objects

private:
    string firstname;
    string lastname;
    string phonenum;
    int id;
};

// inputs information into the struct
istream& operator>>(istream& in, Data& r) {

    r.id++;
    return in >> r.firstname >> r.lastname >> r.phonenum;

}

// outputs information from the struct
ostream& operator<<(ostream& out, const Data& r) {

    string name = r.lastname + ", " + r.firstname;
    return cout << left << setw(24) << name << setw(12) << r.phonenum << setw(9) << right << r.id  << '\n';

}

// Sorts info structs by lastname, then firstname, and then finally phone number
bool Data::operator<(const Data& rhs) const
{
    if (lastname <= rhs.lastname){
		if(lastname == rhs.lastname){
			if(firstname <= rhs.firstname){
				if(firstname == rhs.firstname){
					if(phonenum <= rhs.phonenum){
						return phonenum < rhs.phonenum;
					}
					return true;
				}
				return true;
			}
			return true;
		}
		return true;
	}
	return false;
}

// Prints out vector of info in neat format
template <typename T>
void printlist(T begin, T end) {

    T it;

    for (it = begin; it != end; ++it) {
        cout << *it;
    }
}

int main(int argc, char* argv[]) {

    // variables
    ifstream file;
    string mode;
    slist<Data> A;
    Data din;

    // if argc is less than 3 and argv[1] is not equal to quicksort and mergesort give the user proper usage message
    if (argc < 3 || (strcmp("-mergesort", argv[1]) && strcmp("-quicksort", argv[1]))) {
        cerr << "Improper usage. Use './Slist -[mergesort | quicksort] [filename]'\n";
        return 1;
    }
    mode = argv[1];
    // open file
    file.open(argv[2]);

    // check if file is open
    if (!file.is_open()) {
        cerr << "Invalid file given!\n";
        return 1;
    }

    // input file info into din
    while (file >> din) {

        // add to linked list
        A.push_back(din);
    }

    // close file
    file.close();

    // sort and then print
    A.sort(mode);
    printlist(A.begin(), A.end());

}
