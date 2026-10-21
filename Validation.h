// Validation.h - small helper functions used by every module
// (input validation + safe input reading + file-line splitting)
#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
#include <vector>
using namespace std;

// ----- validation -----
bool isValidEmail(const string& email);
bool isValidPassword(const string& password);   // minimum 8 characters
bool isValidPhone(const string& phone);         // exactly 10 digits
bool isValidUsername(const string& username);   // letters, digits, underscore

// ----- string helpers -----
string toLowerCase(string s);
// Splits "a|b|c" into parts. The LAST part keeps everything that is left.
vector<string> splitLine(const string& line, int parts);

// ----- input helpers -----
void readLine(string& s);                // reads a full line (spaces allowed)
void readText(string& s);                // same, but replaces '|' (used as file separator)
int readNumber(const string& prompt, int minValue, int maxValue);

#endif
