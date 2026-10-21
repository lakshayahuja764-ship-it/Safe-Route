// Facility.h - a hospital or police station
// NOTE: facility data in this project is predefined SAMPLE data (no GPS, not live).
#ifndef FACILITY_H
#define FACILITY_H

#include <string>
using namespace std;

class Facility
{
private:
    int id;
    string name;
    string type;     // "Hospital" or "Police Station"
    string area;     // must match a location name in the graph
    string phone;    // demo number only

public:
    Facility();
    Facility(int i, string n, string t, string a, string p);

    int getId() const;
    string getName() const;
    string getType() const;
    string getArea() const;
    string getPhone() const;

    void display() const;
};

#endif
