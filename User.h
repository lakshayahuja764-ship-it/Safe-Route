// User.h - one registered account
#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User
{
private:                       // encapsulation: data cannot be changed directly from outside
    string name;
    string username;
    string email;
    string password;

public:
    User();                                                   // default constructor
    User(string n, string u, string e, string p);             // parameterized constructor

    string getName() const;
    string getUsername() const;
    string getEmail() const;

    void setName(string n);
    void setEmail(string e);
    void setPassword(string p);

    bool checkPassword(string p) const;   // password is never returned, only compared
    string toFileString() const;          // name|username|email|password
};

#endif
