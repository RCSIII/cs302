// Robert Schenck
// Lab 2
// 2/4/2021

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

class info {
public:
    // add operator< using lastname, firstname, phone number

    friend istream& operator>>(istream&, info&);
    friend ostream& operator<<(ostream&, const info&);
    bool operator<(const info&) const;

private:
    string firstname;
    string lastname;
    string phonenum;
};

// inputs information into the struct
istream& operator>>(istream& in, info& r) {

    return in >> r.firstname >> r.lastname >> r.phonenum;

}

// outputs information from the struct
ostream& operator<<(ostream& out, const info& r) {

    string name = r.lastname + ", " + r.firstname;
    return cout << left << setw(24) << name << setw(12) << r.phonenum << '\n';

}

template <typename Tdata>
void quicksort(vector<Tdata>& A, int left, int right) {

    if (left < right) {
        // select pivot: random
        int randomIndex = left + rand() % (right - left);

        swap(A[randomIndex], A[right]);
        Tdata pivot = A[right];


        int pindex = left - 1;

        for (int j = left; j <= right - 1; j++) {

            if (A[j] < pivot) {
                pindex++;
                swap(A[pindex], A[j]);
            }
        }

        swap(A[pindex + 1], A[right]);
        pindex += 1;

        quicksort(A, left, pindex - 1);
        quicksort(A, pindex + 1, right);
    }
    
}

template <typename Tdata>
void quickselect(vector<Tdata> &A, int k, int left, int right) {

    while (1) {
        // select pivot: random
        int randomIndex = left + rand() % (right - left);

        swap(A[randomIndex], A[right]);
        Tdata pivot = A[right];


        int pindex = left - 1;

        for (int j = left; j <= right - 1; j++) {

            if (A[j] < pivot) {
                pindex++;
                swap(A[pindex], A[j]);
            }
        }

        swap(A[pindex + 1], A[right]);
        pindex += 1;

        if (pindex == k) {
            return;
        }

        if (k < pindex) {
            right = pindex - 1;
        }
        else {
            left = pindex + 1;
        }
    }
}

// Prints out vector of info in neat format
template <typename T>
void printlist(T begin, T end) {

    T it;

    for (it = begin; it != end; it++) {
        cout << *it;
    }
}

int main(int argc, char* argv[]) {
    // usage: Qsort -stl | -rpivot [k0 k1] file.txt
    // Variable declarations
    ifstream file;
    string line;
    string word;
    string mode;
    int k0 = 0, k1 = 0;
    istringstream iss;
    vector<info> info_vector;


    mode = argv[1];
    // error checking for correct amount of arguments and file name
    // determines the command line argument given
    if (argc < 3) {
        cerr << "Improper usage.\n";
        return 1;
    }
    if (argc == 5) {
        file.open(argv[4]);
    }
    else {
        file.open(argv[2]);
    }

    // Open file for reading
    if (!file.is_open()) {
        cerr << "Invalid file given!\n";
        return 1;
    }

    // reads in data from the file and inserts it into the list
    while (!(file.eof())) {
        info info;
        getline(file, line);
        iss.str(line);

        // inserts data into struct
        iss >> info;

        iss.clear();
        info_vector.push_back(info);
    }
    file.close();
    info_vector.pop_back();

    // perform command-line error checking
    if (argc == 5) {
        if (atoi(argv[2]) < 0) {
            k0 = 0;
        }
        else {
            k0 = atoi(argv[2]);
        }
        if (atoi(argv[3]) > (signed)info_vector.size()) {
            k1 = info_vector.size();
        }
        else {
            k1 = atoi(argv[3]);
        }
    }

    if (mode == "-stl") {
        sort(info_vector.begin(), info_vector.end());
        printlist(info_vector.begin(), info_vector.end());
    }
    else if (mode == "-rpivot") {
        quicksort(info_vector, 0, info_vector.size() - 1);
        printlist(info_vector.begin(), info_vector.end());
    }
    else if (mode == "-rpivot" && argc == 5) {
        quickselect(info_vector, 0, k0, info_vector.size() - 1);
        quickselect(info_vector, k0, k1, info_vector.size() - 1);
        printlist(info_vector.begin(), info_vector.end());
    }
}

// Sorts info structs by lastname, then firstname, and then finally phone number
bool info::operator<(const info&rhs) const
{
    if (lastname <= rhs.lastname) {
        if (lastname == rhs.lastname) {
            if (firstname <= rhs.firstname) {
                if (firstname == rhs.firstname) {
                    if (phonenum <= rhs.phonenum) {
                        return true;
                    }
                }
                return true;
            }
        }
        return true;
    }
    return false;
}

