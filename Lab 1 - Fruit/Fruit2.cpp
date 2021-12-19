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

// Fruit structure
struct fruit {

    string name;
    float numPurchased;
    float unitCost;
    bool operator<(const fruit&) const;
    bool operator<=(const fruit&) const;
    int operator+=(const fruit&);
    bool operator==(const fruit&) const;
};

// node structure
struct node {
    
    fruit data;

    node* parent;
    node* next;

};

// insert function, depending on the commmand line arguments given can sort ordered or unordered, compressed or uncompressed
void insert(node* headNode, fruit newFruit, bool inorder, bool sortone, bool sortall) {
    node* new_node = new node;

    new_node->data.name = newFruit.name;
    new_node->data.numPurchased = newFruit.numPurchased;
    new_node->data.unitCost = newFruit.unitCost;

    while (headNode->next != NULL) {
        if (sortall) {
            if (new_node->data < headNode->next->data) {
                new_node->parent = headNode;
                new_node->next = headNode->next;
                headNode->next->parent = new_node;
                headNode->next = new_node;
                return;
            }
        }
        if (sortone) {
            if (new_node->data <= headNode->next->data) {
                if (headNode->next->data == new_node->data) {
                    headNode->next->data += new_node->data;
                    delete new_node;
                    return;
                }
                new_node->parent = headNode;
                new_node->next = headNode->next;
                headNode->next->parent = new_node;
                headNode->next = new_node;
                return;
            }
        }
        headNode = headNode->next;
    }

    headNode->next = new_node;
    new_node->parent = headNode;


}

// cleans up memory after the program is finished
void cleanup(node* head) {

    if (head) {
        cleanup(head->next);
        delete head;
        head = NULL;
    }

}

int main(int argc, char* argv[])
{

    // Variable declarations
    ifstream file;
    string line;
    string word;
    string mode;
    istringstream iss;
    float fruitCost = 0.0;
    float totalCost = 0.0;
    bool sortall = false;
    bool inorder = false;
    bool sortone = false;
    node* head = new node;

    // error checking for correct amount of arguments and file name
    if (argc < 3) {
        cerr << "Improper usage.\n";
        return 1;
    }
    file.open(argv[2]);
    if (!file.is_open()) {
        cerr << "Invalid file given!\n";
        return 1;
    }

    mode = argv[1];

    // determines the command line argument given
    if (mode == "-sortall") {
        sortall = true;
    }
    else if (mode == "-inorder") {
        inorder = true;
    }
    else if (mode == "-sortone") {
        sortone = true;
    }
    else {
        cerr << "Improoper usage.\n";
    }

    // reads in data from the file and inserts it into the list
    while (!(file.eof())) {
        fruit fruits;
        getline(file, line);
        iss.str(line);

        // inserts data into struct
        iss >> fruits.name;
        iss >> fruits.numPurchased;
        iss >> fruits.unitCost;

        iss.clear();

        // inserts fruit into list
        if (fruits.name != "") {
            insert(head, fruits, inorder, sortone, sortall);
        }
    }
    file.close();

    // iterates through the list and prints it off neatly
    while (head->next != NULL) {
        fruitCost = head->next->data.unitCost * head->next->data.numPurchased;
        totalCost += fruitCost;
        cout << setw(20) << setfill('.') << left << head->next->data.name << " ";
        cout << setw(5) << setfill(' ') << right << fixed << setprecision(2) << head->next->data.numPurchased;
        cout << " x " << setw(4) << fixed << head->next->data.unitCost;
        cout << " = " << setw(7) << setprecision(2) << fruitCost << "  : ";
        cout << setw(8) << right << setprecision(2) << totalCost << '\n';
        head->next = head->next->next;
    }

    cleanup(head);

}

// compares the name of the fruit being added
bool fruit::operator<(const fruit& rhs) const
{
    if (name < rhs.name) {
        return true;
    }

    return false;
}

// compares the name of the fruit being added
bool fruit::operator<=(const fruit& rhs) const
{
    if (name <= rhs.name) {
        return true;
    }

    return false;
}

int fruit::operator+=(const fruit& rhs)
{
    return numPurchased += rhs.numPurchased;
}

bool fruit::operator==(const fruit& rhs) const
{
    if (name == rhs.name) {
        return true;
    }

    return false;
}
