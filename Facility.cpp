// Facility.cpp
#include "Facility.h"
#include <iostream>
#include <iomanip>
using namespace std;

Facility::Facility()
{
    id = 0;
    name = "";
    type = "";
    area = "";
    phone = "";
}

Facility::Facility(int i, string n, string t, string a, string p)
{
    id = i;
    name = n;
    type = t;
    area = a;
    phone = p;
}

int Facility::getId() const { return id; }
string Facility::getName() const { return name; }
string Facility::getType() const { return type; }
string Facility::getArea() const { return area; }
string Facility::getPhone() const { return phone; }

void Facility::display() const
{
    cout << left << setw(5) << id << setw(24) << name << setw(16) << type
         << setw(18) << area << phone << endl;
}
