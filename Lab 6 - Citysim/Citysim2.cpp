#define _USE_MATH_DEFINES
#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <limits>
#include <stack>
#include <sstream>

using namespace std;

// City class definition
class city {

public:

    string getName() { return name; };
    string getType() { return type; };
    int getZone() { return zone; };
    int getPopulation() { return population; };
    float getLatitude() { return latitude; };
    float getLongitude() { return longitude; };

    friend istream& operator>>(istream&, city&);
    friend ostream& operator<<(ostream&, city&);


private:
    string name, type;
    int zone, population;
    float latitude, longitude;
};

istream& operator>>(istream& fin, city& city){

    // Handling creating a city object using an input stream
    fin >> city.zone >> city.name >> city.type >> city.latitude >> city.longitude >> city.population;
    city.latitude = city.latitude * (M_PI / 180);
    city.longitude = city.longitude * (M_PI / 180);

    return fin;
}

ostream& operator<<(ostream& fout, city& city){

    // Handling for printing out a object of the city class
    fout << setw(18) << left << city.name;
    fout << setw(12) << left << city.type;
    fout << setw(2) << left << city.zone;
    fout << setw(10) << right << city.population;

    city.latitude = city.latitude * (180 / M_PI);
    fout << setw(8) << right << fixed << setprecision(2) << city.latitude;

    city.longitude = city.longitude * (180 / M_PI);
    fout << setw(8) << right << fixed << setprecision(2) << city.longitude;

    return fout;
}

bool read_cityinfo(vector<city>& cities, map<string, int> &cityNames) {
    ifstream ifs;
    string line;
    istringstream iss;
    city temp;

    // Open file for reading
    ifs.open("city_list.txt");

    // Error handling
    if (!ifs.is_open()) {
        cerr << "Error opening file while reading.\n";
        return false;
    }

    // Go through each line in the file, ignoring empty ones and ones that start with #, and storing them in a vector<city>
    while (getline(ifs, line)) {
        if (!(line.size() == 0 || line[0] == '#')) {
            iss.str(line);
            iss >> temp;
            cities.push_back(temp);
            iss.clear();
        }
    }

    ifs.close();

    for (int i = 0; i < (int)cities.size(); i++) {
        cityNames.insert(pair<string, int>(cities[i].getName(), i));
    }
    return true;
}

void write_cityinfo(vector<city>& cities) {
    ofstream ofs;

    // Open file for writing
    ofs.open("city_info.txt");
    
    // Error handling
    if (!ofs.is_open()) {
        cerr << "Error opening file while writing.\n";
        exit(0);
    }

    // Pretty print information for all cities to "city_info.txt"
    ofs << "CITY INFO (N=" << cities.size() << "):\n\n";
    for (int i = 0; i < (int)cities.size(); i++) {
        ofs << setw(3) << right << i << " ";
        ofs << cities[i];
        ofs << '\n';
    }

    ofs.close();
}

// costtables class definitions
class costtables {
public:
    costtables(vector<city>& cities);
    float operator()(int, int, int);

private:
    vector<float> timeDistanceTables[2];
};


costtables::costtables(vector<city>& cities) {

    // Constant for Earth's radius
    const float earthRadius = 3982.0f;

    // Initialize the time and distance tables
    timeDistanceTables[0].assign((cities.size() * (cities.size() + 1)) / 2, 0.0f);
    timeDistanceTables[1].assign((cities.size() * (cities.size() + 1)) / 2, 0.0f);

    // Variables to simpilify the caclulation
    float iLatitude, iLongitude, jLatitude, jLongitude;
    float longitudeDifference, latitudeDifference, latitudeProduct;
    float unroundedDistance, roundedDistance;

    for (int i = 0; i < (int)cities.size(); i++) {
        for (int j = 0; j < i; j++) {
            // Index to insert into
            int index = i * (i + 1) / 2 + j;

            // Get the latitudes and longitude for i and j
            iLatitude = cities[i].getLatitude();
            iLongitude = cities[i].getLongitude();
            jLatitude = cities[j].getLatitude();
            jLongitude = cities[j].getLongitude();

            // Get the latitude difference, longitude difference, and latitude product
            latitudeDifference = abs((jLatitude - iLatitude) / 2.0f);
            longitudeDifference = abs((jLongitude - iLongitude) / 2.0f);
            latitudeProduct = cos(iLatitude) * cos(jLatitude);

            // Get the unrounded difference using the Great-circle distance formula
            unroundedDistance = 2.0f * asin(sqrt(pow(sin(latitudeDifference), 2.0f) + latitudeProduct * pow(sin(longitudeDifference), 2.0f)));
            // Round answer to the nearest 10
            roundedDistance = 10.0f * round(earthRadius * unroundedDistance / 10.0f);

            // Store into table
            timeDistanceTables[0][index] = roundedDistance;

            // Caclulate time to travel from i to j
            if (cities[i].getType() == "GATEWAY" || cities[j].getType() == "GATEWAY") {
                // For air travel
                timeDistanceTables[1][index] = roundedDistance / 570;
            }
            else {
                // For ground travel
                timeDistanceTables[1][index] = roundedDistance / 60;
            }
        }
    }

}

float costtables::operator()(int mode, int i, int j)
{
    // if i is less than j then swap them
    int temp;
    if (i < j) {
        temp = i;
        i = j;
        j = temp;
    }

    // Return value at calcluated index, if mode is 0 we return a value from the distance table, if 1 we return a value from the time table
    if (mode == 0) {
        return timeDistanceTables[0][i * (i + 1) / 2 + j];
    }
    else if (mode == 1) {
        return timeDistanceTables[1][i * (i + 1) / 2 + j];
    }

    return 0.0f;
}

void write_distancetable(vector<city>& cities, costtables costT) {
    ofstream ofs;
    string cityToCity;

    // Open file for writing
    ofs.open("city_distancetable.txt");

    // Error handling
    if (!ofs.is_open()) {
        cerr << "Error opening file while writing.\n";
        exit(0);
    }

    // Pretty print the distance table results to "city_distancetable.txt"
    ofs << "DISTANCE TABLE:\n";
    for (int i = 0; i < (int)cities.size(); i++) {
        for (int j = 0; j < i; j++) {
            ofs << setw(3) << right << i << " ";
            cityToCity = cities[i].getName() + " to " + cities[j].getName() + " ";
            ofs << setw(38) << setfill('.') << left << cityToCity;
            ofs << setw(7) << setfill(' ') << right << fixed << setprecision(1) << costT(0, i, j);
            ofs << " miles\n";
        }
        ofs << '\n';
    }
}

void write_timetable(vector<city>& cities, costtables costT) {
    ofstream ofs;
    string cityToCity;

    // Open file for writing
    ofs.open("city_timetable.txt");

    // Error handling
    if (!ofs.is_open()) {
        cerr << "Error opening file while writing.\n";
        exit(0);
    }

    // Pretty print the time table results to "city_timetable.txt"
    ofs << "TIME TABLE:\n";
    for (int i = 0; i < (int)cities.size(); i++) {
        for (int j = 0; j < i; j++) {
            ofs << setw(3) << right << i << " ";
            cityToCity = cities[i].getName() + " to " + cities[j].getName() + " ";
            ofs << setw(38) << setfill('.') << left << cityToCity;
            ofs << setw(5) << setfill(' ') << right << fixed << setprecision(1) << costT(1, i, j);
            ofs << " hours\n";
        }
        ofs << '\n';
    }
}

// Gets the nearest gateway city in a different zone for the given city
int getNearestGateway(vector<city>& cities, costtables costT, int zone, int i) {
    float lowestDistance = 10000.0f;
    int k = -1;

    // Loop through every city and if they are both gateway cities and the zone matches with the zone asked for
    // We compare distances and if it smaller set it as the new lowest distance and get it's index
    for (int j = 0; j < (int)cities.size(); j++) {
        if (zone == cities[j].getZone() && cities[j].getType() == "GATEWAY" && cities[i].getType() == "GATEWAY") {
            if (costT(0, i, j) < lowestDistance) {
                lowestDistance = costT(0, i, j);
                k = j;
            }
        }
    }

    // Return the index 
    return k;
}

vector<vector<int> > create_citygraph(vector<city>& cities, costtables costT) {
    vector<vector<int> > indexes;
    indexes.assign(cities.size(), vector<int>(0, 0));
    float lowestDistance = 10000.0f;
    int k = -1;

    // Loops through every city
    for (int i = 0; i < (int)cities.size(); i++) {
        for (int j = 0; j < (int)cities.size(); j++) {

            // If j is not i
            if (j != i) {
                // If they are the same zone and same type then add them to the adjacency list
                if (cities[i].getZone() == cities[j].getZone()) {
                    if (cities[i].getType() == cities[j].getType()) {
                        indexes[i].push_back(j);
                    }
                }
                // If they are in the same zone and but of different types, then test for nearest one and at the end of the loop we add the nearest one
                if ((cities[i].getZone() == cities[j].getZone()) && (cities[i].getType() == "REGIONAL" && cities[j].getType() == "GATEWAY")) {
                    if (costT(0, i, j) < lowestDistance) {
                        lowestDistance = costT(0, i, j);
                        k = j;
                    }
                }
                // If the cities are in different zone and they are both gateways...
                if ((cities[i].getZone() != cities[j].getZone()) && (cities[i].getType() == "GATEWAY" && cities[j].getType() == "GATEWAY")) {
                    // Get the nearest gateway city that is in a different zone
                    int  a = getNearestGateway(cities, costT, cities[j].getZone(), i);
                    if (a != -1) {
                        // If less than 5200 miles then add to adjacency list
                        if (costT(0, i, a) < 5200.0f) {
                            indexes[i].push_back(a);
                            indexes[a].push_back(i);
                        }
                    }
                }
            }
        }
        // Add nearest gateway to regional's adjacency list
        if (k != -1) {
            indexes[i].push_back(k);
            indexes[k].push_back(i);
        }
        k = -1;
        lowestDistance = 10000.0f;
    }

    // Sort the list and get rid of duplicates
    for (int i = 0; i < (int)cities.size(); i++) {
        sort(indexes[i].begin(), indexes[i].end());
        indexes[i].resize(unique(indexes[i].begin(), indexes[i].end()) - indexes[i].begin());
    }

    return indexes;
}

void write_citygraph(vector<city>& cities, costtables costT, vector<vector<int> > indexes) {
    ofstream ofs;

    // Open file for writing
    ofs.open("city_graph.txt");

    // Error handling
    if (!ofs.is_open()) {
        cerr << "Error opening file while writing.\n";
        exit(0);
    }

    // Loops through the graph and print out all adjancencies
    ofs << "CITY GRAPH:\n\n";
    for (int i = 0; i < (int)indexes.size(); i++) {
        ofs << setw(3) << right << i << " " << cities[i].getName() << '\n';
        for (int j = 0; j < (int)indexes[i].size(); j++) {
            ofs << setw(6) << right << indexes[i][j] << " " << setw(18) << left << cities[indexes[i][j]].getName();
            ofs << setw(8) << right << fixed << setprecision(1) << costT(0, i, indexes[i][j]) << " miles";
            ofs << setw(6) << right << fixed << setprecision(1) << costT(1, i, indexes[i][j]) << " hours\n";
        }
        if ((int)indexes.size() - 1 != i) {
            ofs << '\n';
        }
    }
}

void dijkstra_route(int source, int sink, vector<city> cities, vector<vector<int> > adjacencyList, costtables costT, int mode) {

    vector<bool> vcolor;
    vector<float> vdist;
    vector<float> vtime;
    vector<int> vlink;

    vcolor.assign(cities.size(), true);
    if (mode == 0) {
        vdist.assign(cities.size(), numeric_limits<float>::max());
        vtime.assign(cities.size(), 0.0f);
        vdist[source] = 0.0f;
    }
    else {
        vdist.assign(cities.size(), 0.0f);
        vtime.assign(cities.size(), numeric_limits<float>::max());
        vtime[source] = 0.0f;
    }
    vlink.assign(cities.size(), -1);

    vlink[source] = source;

    while (1) {
        int next_i = -1;
        float mindist = numeric_limits<float>::max();

        for (int i = 0; i < (int)vcolor.size(); i++) {
            if (mode == 0) {
                if (vcolor[i] == true && mindist > vdist[i]) {
                    next_i = i;
                    mindist = vdist[i];
                }
            }
            else {
                if (vcolor[i] == true && mindist > vtime[i]) {
                    next_i = i;
                    mindist = vtime[i];
                }
            }
        }

        int i = next_i;
        if (i == -1) {
            return;
        }

        vcolor[i] = false;

        if (i == sink) {
            break;
        }

        for (int k = 0; k < (int)adjacencyList[i].size(); k++) {
            int j = adjacencyList[i][k];
            float wij = costT(0, i, j);
            float wij2 = costT(1, i, j);
            if (vcolor[j] == true) {
                if (mode == 0) {
                    if (vdist[j] > vdist[i] + wij) {
                        vdist[j] = vdist[i] + wij;
                        vtime[j] = vtime[i] + wij2;
                        vlink[j] = i;
                    }
                }
                else {
                    if (vtime[j] > vtime[i] + wij2) {
                        vdist[j] = vdist[i] + wij;
                        vtime[j] = vtime[i] + wij2;
                        vlink[j] = i;
                    }
                }
            }
        }
    }

    if (mode == 0) {
        if (vdist[sink] == numeric_limits<float>::max()) {
            cout << "No path from " << cities[source] << " to " << cities[sink] << '\n';
            return;
        }
    }
    else {
        if (vtime[sink] == numeric_limits<float>::max()) {
            cout << "No path from " << cities[source] << " to " << cities[sink] << '\n';
            return;
        }
    }

    stack<int> S;

    for (int i = sink; i != source; i = vlink[i]) {
        S.push(i);
    }
    S.push(source);

    bool start = true;
    int j = 0;

    while (!S.empty()) {
        if (start == true) {
            j = S.top();
            start = false;
        }
        int i = S.top();
        S.pop();

        cout << setw(7) << right << fixed << setprecision(1) << vdist[i] << " miles ";
        cout << setw(5) << right << fixed << setprecision(1) << vtime[i] << " hours : ";
        cout << setw(3) << i << " " << setw(18) << left << cities[i].getName();
        if (i == source) {
            cout << '\n';
        }
        else {
            cout << setw(8) << right << fixed << setprecision(1) << costT(0, i, j) << " miles ";
            cout << setw(8) << right << fixed << setprecision(1) << costT(1, i, j) << " hours \n";
        }
        j = i;
    }
}

int main(int argc, char* argv[]){

    // Command Line Parsing
    if (argc != 2) {
        cerr << "Usage: ./Citysim1 -graphinfo|distance|time\n";
        return 1;
    }

    string mode = argv[1];
    vector<city> cities;
    vector<vector<int> > adjacencyList;
    map<string, int> cityIndexes;

    // Read in city information
    if (!read_cityinfo(cities, cityIndexes)) {
        return 1;
    }

    // Calculate Distance and Time from City to City
    costtables costT(cities);
    adjacencyList = create_citygraph(cities, costT);

    // Print out results
    if (!strcmp(argv[1], "-graphinfo")) {
        write_cityinfo(cities);
        write_timetable(cities, costT);
        write_distancetable(cities, costT);
        write_citygraph(cities, costT, adjacencyList);

        return 0;
    }

    string from, to;
    string line;
    istringstream iss;
    map<string, int>::iterator itFrom;
    map<string, int>::iterator itTo;
    int distanceOrTime;

    if (!strcmp(argv[1], "-distance") || !strcmp(argv[1], "-time")) {

        if (!strcmp(argv[1], "-distance")) {
            distanceOrTime = 0;
        }
        if (!strcmp(argv[1], "-time")) {
            distanceOrTime = 1;
        }

        cout << "Enter> ";
        while (getline(cin, line)) {

            iss.str(line);
            iss >> from >> to;
            iss.clear();
            cout << '\n';

            itFrom = cityIndexes.find(from);
            if (itFrom == cityIndexes.end()) {
                cout << from << " is not a city!\n";
                continue;
            }
            itTo = cityIndexes.find(to);
            if (itTo == cityIndexes.end()) {
                cout << to << " is not a city!\n";
                continue;
            }

            dijkstra_route(itFrom->second, itTo->second, cities, adjacencyList, costT, distanceOrTime);
            cout << '\n';
            cout << "Enter> ";
        }
        cout << '\n';
    }
}
