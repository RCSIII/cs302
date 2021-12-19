#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>

using namespace std;

struct cell {
    int row, col, direction;

    cell() { row = 0; col = 0; direction = 0; }
    cell(int row, int col, int direction) {
        this->row = row;
        this->col = col;
        this->direction = direction;
    }
};

template <typename T>
class matrix {
public:
    matrix() { data = NULL;  }
    ~matrix() {
        if (data) {
            if (data[0]) {
                delete[] data[0];
            }
            delete[] data;

            data = NULL;
        }
    }

    void assign(int _Nrows, int _Ncols, const T& value = T()) {
        Nrows = _Nrows;
        Ncols = _Ncols;

        data = new T * [Nrows];
        data[0] = new T[Nrows * Ncols];

        for (int i = 1; i < Nrows; i++) {
            data[i] = data[i - 1] + Ncols;
        }

        for (int i = 0; i < Nrows; i++) {
            for (int j = 0; j < Ncols; j++) {
                data[i][j] = value;
            }
        }
    }

    int get_Nrows() const { return Nrows; }
    int get_Ncols() const { return Ncols; }

    T* operator[] (int i) { return data[i]; }

private:
    int Nrows, Ncols;
    T** data;
};

class LCS {
public:

    void text1_push_back(string);
    void text2_push_back(string);
    void compute_alignment();
    void report_difference();

private:
    void report_difference(int, int);
    void print_dump(vector<cell> deleteVec, vector<cell> insertVec);
    void print_lines(vector<cell> vec, string character);
    void print_both(vector<cell> deleteVec, vector<cell> insertVec);
    vector<string> text1, text2;
    int m, n;
    string et1, et2;
    matrix<int> cost; // edit costs
    matrix<int> link; // alignment info
    stack<cell> trace;

};

void LCS::text1_push_back(string str) {
    text1.push_back(str);
}

void LCS::text2_push_back(string str) {
    text2.push_back(str);
}

void LCS::compute_alignment() {

    text1.insert(text1.begin(), "");
    text2.insert(text2.begin(), "");

    int m = text1.size();
    int n = text2.size();

    cost.assign(m, n);
    link.assign(m, n);

    cost[0][0] = 0;
    link[0][0] = 0;

    for (int i = 1; i < m; i++) {
        cost[i][0] = cost[i - 1][0] + 1;
        link[i][0] = 1;
    }

    for (int j = 1; j < n; j++) {
        cost[0][j] = cost[0][j - 1] + 1;
        link[0][j] = 2;
    }

    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            cost[i][j] = cost[i - 1][j - 1] + (text1[i] == text2[j] ? 0 : m + n);
            link[i][j] = 4;

            int delcost = cost[i - 1][j] + 1;
            if (delcost < cost[i][j]) {
                cost[i][j] = delcost;
                link[i][j] = 1;
            }
            int inscost = cost[i][j - 1] + 1;
            if (inscost < cost[i][j]) {
                cost[i][j] = inscost;
                link[i][j] = 2;
            }
        }
    }
}

void LCS::report_difference(int i, int j) {

    cell c;

    if (i == 0 && j == 0) {
        return;
    }

    if (link[i][j] == 4) {
        c.row = i;
        c.col = j;
        c.direction = 4;
        i -= 1;
        j -= 1;
    }
    else if (link[i][j] == 1) {
        c.row = i;
        c.col = j;
        c.direction = 1;
        i -= 1;
    }
    else if (link[i][j] == 2) {
        c.row = i;
        c.col = j;
        c.direction = 2;
        j -= 1;
    }

    trace.push(c);

    report_difference(i, j);
}

void LCS::print_dump(vector<cell> deleteVec, vector<cell> insertVec){

    if (deleteVec.size() == 0) {
        print_lines(insertVec, "a");
        for (unsigned int i = 0; i < insertVec.size(); i++) {
            cout << "> " << text2[insertVec[i].col] << '\n';
        }
    }
    else if (insertVec.size() == 0) {
        print_lines(deleteVec, "d");
        for (unsigned int i = 0; i < deleteVec.size(); i++) {
            cout << "< " << text1[deleteVec[i].row] << '\n';
        }
    }
    else {
        print_both(deleteVec, insertVec);
        for (unsigned int i = 0; i < deleteVec.size(); i++) {
            cout << "< " << text1[deleteVec[i].row] << '\n';
        }
        cout << "---\n";
        for (unsigned int i = 0; i < insertVec.size(); i++) {
            cout << "> " << text2[insertVec[i].col] << '\n';
        }
    }
}

void LCS::print_lines(vector<cell> vec, string character){
    int i = vec.size();
    if (i == 0) {
        return;
    }
    if (vec[0].row == vec[i - 1].row) {
        cout << vec[0].row << character;
    }
    else {
        cout << vec[0].row << "," << vec[i - 1].row << character;
    }
    if (vec[0].col == vec[i - 1].col) {
        cout << vec[0].col << '\n';
    }
    else {
        cout << vec[0].col << "," << vec[i - 1].col << '\n';
    }
}

void LCS::print_both(vector<cell> deleteVec, vector<cell> insertVec){

    if (deleteVec.size() == 0 || insertVec.size() == 0) {
        return;
    }

    if (deleteVec[0].row < insertVec[0].row) {
        cout << deleteVec[0].row + 1 << ",";
    }
    else {
        cout << insertVec[0].row + 1 << ",";
    }

    if (deleteVec[deleteVec.size() - 1].row > insertVec[insertVec.size() - 1].row) {
        cout << deleteVec[deleteVec.size() - 1].row << "c";
    }
    else {
        cout << insertVec[insertVec.size() - 1].row << "c";
    }

    if (deleteVec[0].col < insertVec[0].col) {
        cout << deleteVec[0].col << ",";
    }
    else {
        cout << insertVec[0].col << ",";
    }

    if (deleteVec[deleteVec.size() - 1].col > insertVec[insertVec.size() - 1].col) {
        cout << deleteVec[deleteVec.size() - 1].col << '\n';
    }
    else {
        cout << insertVec[insertVec.size() - 1].col << '\n';
    }
}

void LCS::report_difference(){

    vector<cell> deleteVec;
    vector<cell> insertVec;

    report_difference(text1.size() - 1, text2.size() - 1);

    while (!trace.empty()) {

        if (trace.top().direction == 1) {
            deleteVec.push_back(trace.top());
        }
        else if (trace.top().direction == 2) {
            insertVec.push_back(trace.top());
        }
        else if (trace.top().direction == 4) {
            print_dump(deleteVec, insertVec);
            deleteVec.clear();
            insertVec.clear();
        }
        trace.pop();
    }
    print_dump(deleteVec, insertVec);

}

int main(int argc, char* argv[]){
    
    string line;
    ifstream file;
    // check two input files are specified on command line
    if (argc != 3) {
        cerr << "Usage: ./Diff8 text1.txt text2.txt\n";
        return 1;
    }
    LCS lcs;  // instantiate your "LCS based diff" object

    // read the text from file1 into the lcs.text1 buffer
    file.open(argv[1]);
    if (!file.is_open()) {
        cerr << "Improper file given!\n";
        return 1;
    }
    while (getline(file, line)) {
        lcs.text1_push_back(line);
    }

    file.close();
    // read the text from file2 into the lcs.text2 buffer
    file.open(argv[2]);

    if (!file.is_open()) {
        cerr << "Improper file given!\n";
        return 1;
    }
    while (getline(file, line)) {
        lcs.text2_push_back(line);
    }
    file.close();
    lcs.compute_alignment();
    lcs.report_difference();

    return 0;
}