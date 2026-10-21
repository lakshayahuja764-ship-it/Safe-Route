// Contact.h - one emergency contact
#ifndef CONTACT_H
#define CONTACT_H

#include <string>
using namespace std;

class Contact
{
private:
    string name;
    string phone;      // string, not int (10 digits + keeps leading zero)
    string relation;

public:
    Contact();
    Contact(string n, string p, string r);

    string getName() const;
    string getPhone() const;
    string getRelation() const;

    void setName(string n);
    void setPhone(string p);
    void setRelation(string r);

    void display() const;
    string toFileString() const;   // name|phone|relation
};

#endif
