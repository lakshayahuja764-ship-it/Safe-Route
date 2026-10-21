// SOS.cpp
#include "SOS.h"
#include "Validation.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
using namespace std;

// ---------------- SOS ----------------
SOS::SOS()
{
    id = 0;
    username = "";
    emergencyType = "";
    location = "";
    priority = "Low";
    status = "Pending";
}

SOS::SOS(int i, string u, string t, string loc, string p, string s)
{
    id = i;
    username = u;
    emergencyType = t;
    location = loc;
    priority = p;
    status = s;
}

int SOS::getId() const { return id; }
string SOS::getUsername() const { return username; }
string SOS::getEmergencyType() const { return emergencyType; }
string SOS::getLocation() const { return location; }
string SOS::getPriority() const { return priority; }
string SOS::getStatus() const { return status; }

void SOS::setStatus(string s) { status = s; }

int SOS::getPriorityLevel() const
{
    if (priority == "Critical") return 4;
    if (priority == "High") return 3;
    if (priority == "Medium") return 2;
    return 1;   // Low
}

void printSOSHeader()
{
    cout << left << setw(6) << "ID" << setw(20) << "Emergency Type" << setw(18) << "Location"
         << setw(10) << "Priority" << "Status" << endl;
    cout << "------------------------------------------------------------------\n";
}

void SOS::displayRow() const
{
    cout << left << setw(6) << id << setw(20) << emergencyType << setw(18) << location
         << setw(10) << priority << status << endl;
}

string SOS::toFileString() const
{
    return to_string(id) + "|" + username + "|" + emergencyType + "|" + location + "|" +
           priority + "|" + status;
}

// ---------------- SOSHistory (linked list) ----------------
SOSHistory::SOSHistory()
{
    head = NULL;
    tail = NULL;
}

SOSHistory::~SOSHistory()
{
    clearList();
}

void SOSHistory::clearList()
{
    Node* current = head;
    while (current != NULL)
    {
        Node* nextNode = current->next;   // remember next before deleting
        delete current;
        current = nextNode;
    }
    head = NULL;
    tail = NULL;
}

// Add a new node at the end of the list.
void SOSHistory::addRecord(const SOS& s)
{
    Node* newNode = new Node(s);
    if (head == NULL)          // empty list
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }
}

// Traverse the list from head to the end and print this user's records. O(n)
int SOSHistory::displayForUser(const string& username) const
{
    int count = 0;
    Node* current = head;
    while (current != NULL)
    {
        if (current->data.getUsername() == username)
        {
            if (count == 0) printSOSHeader();
            current->data.displayRow();
            count++;
        }
        current = current->next;
    }
    return count;
}

// Linear search by SOS ID. Returns a pointer to the stored SOS (so we can change its status).
SOS* SOSHistory::findById(int id, const string& username)
{
    Node* current = head;
    while (current != NULL)
    {
        if (current->data.getId() == id && current->data.getUsername() == username)
            return &(current->data);
        current = current->next;
    }
    return NULL;
}

int SOSHistory::searchByStatus(const string& username, const string& status) const
{
    int count = 0;
    Node* current = head;
    while (current != NULL)
    {
        if (current->data.getUsername() == username && current->data.getStatus() == status)
        {
            if (count == 0) printSOSHeader();
            current->data.displayRow();
            count++;
        }
        current = current->next;
    }
    return count;
}

// Copies this user's records into a vector (used for sorting).
vector<SOS> SOSHistory::getRecordsForUser(const string& username) const
{
    vector<SOS> result;
    Node* current = head;
    while (current != NULL)
    {
        if (current->data.getUsername() == username) result.push_back(current->data);
        current = current->next;
    }
    return result;
}

int SOSHistory::getMaxId() const
{
    int maxId = 0;
    Node* current = head;
    while (current != NULL)
    {
        if (current->data.getId() > maxId) maxId = current->data.getId();
        current = current->next;
    }
    return maxId;
}

void SOSHistory::saveToFile(const string& filename) const
{
    ofstream out(filename.c_str());
    Node* current = head;
    while (current != NULL)
    {
        out << current->data.toFileString() << endl;
        current = current->next;
    }
}

void SOSHistory::loadFromFile(const string& filename)
{
    ifstream in(filename.c_str());
    if (!in) return;               // first run: file does not exist yet

    clearList();
    string line;
    while (getline(in, line))
    {
        vector<string> p = splitLine(line, 6);
        if (p.size() != 6) continue;   // skip damaged lines
        int id = atoi(p[0].c_str());
        addRecord(SOS(id, p[1], p[2], p[3], p[4], p[5]));
    }
}
