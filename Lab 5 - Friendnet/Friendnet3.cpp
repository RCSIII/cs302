#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <set>
#include <time.h>
#include <fstream>
#include <algorithm>
using namespace std;

void set_oldfriends(vector<string>& names, vector<set<int> >& friends, int M0, int M1) {

	int N = (int)names.size();
	int M;
	friends.resize(N);

	// For each name in the list of names
	for (int i = 0; i < N; i++) {

		set<int> doknow;
		// Random amount of friends between 1 and 3
		M = M0 + rand() % M1;

		// While the size of the set is less than the required numbers of friends we need to get
		while ((int)doknow.size() < M) {
			// Get a random name from the list...
			int j = rand() % names.size();
			// and if the random friend is not equal to the current name, insert the random friend into the set
			if (j != i) {
				doknow.insert(j);
			}
		}
		// Insert friends into friend set
		set<int>::iterator iter;
		for (iter = doknow.begin(); iter != doknow.end(); iter++) {
			friends[i].insert(*iter);
			friends[*iter].insert(i);
		}
	}
}

// Set list of people they might know
void set_newfriends(vector<set<int> > friends, vector<set<int> >& new_friends) {

	int N = (int)friends.size();
	new_friends.resize(N);
	// For the number of names
	for (int i = 0; i < N; i++) {
		// For the amount of friends i has
		for (set<int>::iterator j = friends[i].begin(); j != friends[i].end(); ++j) {
			for (set<int>::iterator k = friends[*j].begin(); k != friends[*j].end(); ++k) {
				if (*k != i && friends[i].find(*k) == friends[i].end()) {
					new_friends[i].insert(*k);
				}
			}
		}
	}
}

void writetofile(const char* fname, vector<string>& names, vector<set<int> > friends) {
	ofstream ofs;
	unsigned int maxNameLength = 0;
	int namesPrinted = 0;

	// Open file
	ofs.open(fname);

	// If file is not open exit with error
	if (!ofs.is_open()) {
		cerr << "There was a problem opening this file!\n";
		exit(1);
	}

	// Determine length of longest name
	int N = (int)names.size();
	for (int i = 0; i < N; i++) {
		if (names[i].size() > maxNameLength) {
			maxNameLength = names[i].size();
		}
	}

	// Iterate through every name in the names list and print off all of their friend
	for (int i = 0; i < N; i++) {
		ofs.width(maxNameLength + 1);
		namesPrinted = 0;
		ofs << left << names[i] << ":";
		for (set<int>::iterator it = friends[i].begin(); it != friends[i].end(); ++it) {
			if (namesPrinted % 8 == 0 && namesPrinted > 0) {
				ofs << '\n';
				ofs.width(maxNameLength + 1);
				ofs << left << names[i] << ":";
				namesPrinted = 0;
			}
			ofs << " ";
			ofs.width(maxNameLength);
			ofs << left << names[*it];
			namesPrinted++;
		}
		ofs << '\n';
	}
	// Close file
	ofs.close();
}

int main(int argc, char* argv[]) {

	// Parse command lines arguements
	if (argc == 3) {
		if (strcmp(argv[1], "-seed")) {
			cerr << "Usage: ./Friendnet1 -seed [N]\n";
			return 1;
		}
		else {
			// if valid seed given, seed srand with it
			int seed = atoi(argv[2]);
			srand(seed);
		}
	}
	else if (argc == 1) {
		// seed random with time(NULL) if no seed given
		srand(time(NULL));
	}
	else {
		cerr << "Usage: ./Friendnet1 -seed [N]\n";
	}

	// Read names from stdin into names vector
	vector<string> names;
	string name;
	while (cin >> name) {
		names.push_back(name);
	}

	// Min Friends
	int M0 = 1;
	// Max Friends
	int M1 = 3;

	// 2D array of friends and new friends
	vector<set<int> > friends;
	vector<set<int> > new_friends;

	// Creates a set of friends and writes to doknow1.txt
	set_oldfriends(names, friends, M0, M1);
	writetofile("doknow3.txt", names, friends);

	// Creates a set of new friends and writes to mightknow1.txt
	set_newfriends(friends, new_friends);
	writetofile("mightknow3.txt", names, new_friends);
}