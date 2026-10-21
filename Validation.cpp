// Validation.cpp
#include "Validation.h"
#include <iostream>
#include <cstdlib>
using namespace std;

// Email rule (simple, easy to explain):
//  1. no spaces
//  2. exactly one '@'
//  3. at least one character before '@'
//  4. after '@' there must be a '.' that is not right after '@' and not the last character
bool isValidEmail(const string& email)
{
    int len = email.length();
    if (len < 5) return false;

    for (int i = 0; i < len; i++)
    {
        if (email[i] == ' ') return false;
    }

    size_t at = email.find('@');
    if (at == string::npos) return false;          // no '@'
    if (email.rfind('@') != at) return false;      // more than one '@'
    if (at == 0) return false;                     // nothing before '@'

    size_t dot = email.rfind('.');
    if (dot == string::npos) return false;
    if (dot < at + 2) return false;                // '.' must not be right after '@'
    if (dot == (size_t)len - 1) return false;      // '.' must not be last
    return true;
}

bool isValidPassword(const string& password)
{
    return password.length() >= 8;
}

// Phone is stored as a STRING so that a leading 0 is not lost.
bool isValidPhone(const string& phone)
{
    if (phone.length() != 10) return false;
    for (int i = 0; i < 10; i++)
    {
        if (phone[i] < '0' || phone[i] > '9') return false;
    }
    return true;
}

// Username is also used in a file name, so only safe characters are allowed.
bool isValidUsername(const string& username)
{
    if (username.length() < 3 || username.length() > 20) return false;
    for (size_t i = 0; i < username.length(); i++)
    {
        char c = username[i];
        bool ok = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                  (c >= '0' && c <= '9') || c == '_';
        if (!ok) return false;
    }
    return true;
}

string toLowerCase(string s)
{
    for (size_t i = 0; i < s.length(); i++)
    {
        if (s[i] >= 'A' && s[i] <= 'Z') s[i] = s[i] + 32;
    }
    return s;
}

vector<string> splitLine(const string& line, int parts)
{
    vector<string> result;
    string clean = line;
    if (!clean.empty() && clean[clean.length() - 1] == '\r') clean.erase(clean.length() - 1);

    size_t start = 0;
    for (int i = 0; i < parts - 1; i++)
    {
        size_t pos = clean.find('|', start);
        if (pos == string::npos) break;
        result.push_back(clean.substr(start, pos - start));
        start = pos + 1;
    }
    result.push_back(clean.substr(start));
    return result;   // caller checks result.size() == parts
}

// getline(cin >> ws, s): "ws" skips leftover newline/spaces before reading,
// so an old '\n' in the buffer never gives an empty string.
void readLine(string& s)
{
    if (!getline(cin >> ws, s))
    {
        // input finished (Ctrl+D / Ctrl+Z or end of a test file)
        cout << "\nInput ended. Exiting program.\n";
        exit(0);
    }
    while (!s.empty() && (s[s.length() - 1] == ' ' || s[s.length() - 1] == '\t' || s[s.length() - 1] == '\r'))
    {
        s.erase(s.length() - 1);
    }
}

// '|' separates fields in our text files, so we do not allow it inside normal text.
void readText(string& s)
{
    readLine(s);
    for (size_t i = 0; i < s.length(); i++)
    {
        if (s[i] == '|') s[i] = '-';
    }
}

// Keeps asking until the user types a whole number between minValue and maxValue.
int readNumber(const string& prompt, int minValue, int maxValue)
{
    string s;
    while (true)
    {
        cout << prompt;
        readLine(s);

        bool allDigits = !s.empty() && s.length() <= 6;
        for (size_t i = 0; i < s.length() && allDigits; i++)
        {
            if (s[i] < '0' || s[i] > '9') allDigits = false;
        }

        if (allDigits)
        {
            int value = atoi(s.c_str());
            if (value >= minValue && value <= maxValue) return value;
        }
        cout << "Invalid input. Please enter a number from " << minValue << " to " << maxValue << ".\n";
    }
}
