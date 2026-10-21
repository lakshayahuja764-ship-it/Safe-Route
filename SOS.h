// SOS.h - SOS request, priority-queue comparator, and the linked list for history
#ifndef SOS_H
#define SOS_H

#include <string>
#include <vector>
using namespace std;

// ---------------- SOS class ----------------
class SOS
{
private:
    int id;
    string username;
    string emergencyType;
    string location;
    string priority;     // Critical / High / Medium / Low
    string status;       // Pending / Processed / Cancelled

public:
    SOS();
    SOS(int i, string u, string t, string loc, string p, string s);

    int getId() const;
    string getUsername() const;
    string getEmergencyType() const;
    string getLocation() const;
    string getPriority() const;
    string getStatus() const;

    void setStatus(string s);

    int getPriorityLevel() const;   // Critical=4, High=3, Medium=2, Low=1
    void displayRow() const;        // one line of a table
    string toFileString() const;    // id|username|type|location|priority|status
};

void printSOSHeader();   // table heading used with displayRow()

// ---------------- Comparator for the priority queue ----------------
// std::priority_queue keeps the "largest" element on top.
// operator() returns true when a should come AFTER b (a is less important).
//   - lower priority level  -> comes after
//   - same level            -> bigger ID (newer SOS) comes after, so older SOS is served first
struct CompareSOS
{
    bool operator()(const SOS& a, const SOS& b) const
    {
        if (a.getPriorityLevel() != b.getPriorityLevel())
            return a.getPriorityLevel() < b.getPriorityLevel();
        return a.getId() > b.getId();
    }
};

// ---------------- Linked list node ----------------
struct Node
{
    SOS data;
    Node* next;

    Node(const SOS& s)
    {
        data = s;
        next = NULL;
    }
};

// ---------------- SOS history (singly linked list, written manually) ----------------
class SOSHistory
{
private:
    Node* head;
    Node* tail;      // tail makes adding at the end O(1)

public:
    SOSHistory();
    ~SOSHistory();   // frees all nodes

    void addRecord(const SOS& s);                       // add at end
    int displayForUser(const string& username) const;   // returns how many printed
    SOS* findById(int id, const string& username);      // NULL if not found
    int searchByStatus(const string& username, const string& status) const;
    vector<SOS> getRecordsForUser(const string& username) const;
    int getMaxId() const;

    void saveToFile(const string& filename) const;
    void loadFromFile(const string& filename);

private:
    void clearList();
};

#endif
