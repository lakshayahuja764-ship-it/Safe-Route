// Contact.cpp
#include "Contact.h"
#include <iostream>
#include <iomanip>
using namespace std;

Contact::Contact()
{
    name = "";
    phone = "";
    relation = "";
}

Contact::Contact(string n, string p, string r)
{
    name = n;
    phone = p;
    relation = r;
}

string Contact::getName() const { return name; }
string Contact::getPhone() const { return phone; }
string Contact::getRelation() const { return relation; }

void Contact::setName(string n) { name = n; }
void Contact::setPhone(string p) { phone = p; }
void Contact::setRelation(string r) { relation = r; }

void Contact::display() const
{
    cout << left << setw(20) << name << setw(14) << phone << relation << endl;
}

string Contact::toFileString() const
{
    return name + "|" + phone + "|" + relation;
}
