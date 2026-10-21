// User.cpp
#include "User.h"

User::User()
{
    name = "";
    username = "";
    email = "";
    password = "";
}

User::User(string n, string u, string e, string p)
{
    name = n;
    username = u;
    email = e;
    password = p;
}

string User::getName() const { return name; }
string User::getUsername() const { return username; }
string User::getEmail() const { return email; }

void User::setName(string n) { name = n; }
void User::setEmail(string e) { email = e; }
void User::setPassword(string p) { password = p; }

bool User::checkPassword(string p) const
{
    return password == p;
}

string User::toFileString() const
{
    return name + "|" + username + "|" + email + "|" + password;
}
