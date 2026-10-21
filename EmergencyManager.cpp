// EmergencyManager.cpp
#include "EmergencyManager.h"
#include "Validation.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

const string USERS_FILE = "users.txt";
const string HISTORY_FILE = "sos_history.txt";

// =====================================================
// Constructor and sample data
// =====================================================

/* =====================================================
   Constructor and sample data
   ===================================================== */

EmergencyManager::EmergencyManager()
{
    loggedIn = false;
    currentUser = "";

    loadSampleFacilities();
    loadSampleGraph();
    loadUsers();

    history.loadFromFile(HISTORY_FILE);

    // Continue SOS numbering from previous records
    nextSosId = history.getMaxId() + 1;

    // First SOS ID is 101
    if (nextSosId < 101)
    {
        nextSosId = 101;
    }
}

/* =====================================================
   Dehradun Hospitals and Police Stations
   Phone numbers are demonstration values only.
   ===================================================== */

void EmergencyManager::loadSampleFacilities()
{
    facilities.clear();

    // Hospitals in Dehradun
    facilities.push_back(
        Facility(1, "Doon Hospital",
                 "Hospital", "Patel Nagar",
                 "DEMO-1001"));

    facilities.push_back(
        Facility(2, "Coronation Hospital",
                 "Hospital", "Dalanwala",
                 "DEMO-1002"));

    facilities.push_back(
        Facility(3, "Max Super Speciality Hospital",
                 "Hospital", "Mussoorie Road",
                 "DEMO-1003"));

    facilities.push_back(
        Facility(4, "Synergy Hospital",
                 "Hospital", "Ballupur",
                 "DEMO-1004"));

    // Police stations in Dehradun
    facilities.push_back(
        Facility(5, "Patel Nagar Police Station",
                 "Police Station", "Patel Nagar",
                 "DEMO-2001"));

    facilities.push_back(
        Facility(6, "Dalanwala Police Station",
                 "Police Station", "Dalanwala",
                 "DEMO-2002"));

    facilities.push_back(
        Facility(7, "Nehru Colony Police Station",
                 "Police Station", "Nehru Colony",
                 "DEMO-2003"));

    facilities.push_back(
        Facility(8, "Rajpur Police Station",
                 "Police Station", "Rajpur Road",
                 "DEMO-2004"));
}

/* =====================================================
   Dehradun Sample Road Graph
   Distances are illustrative, not verified road distances.
   ===================================================== */

void EmergencyManager::loadSampleGraph()
{
    // Dehradun locations
    graph.addLocation("ISBT Dehradun");
    graph.addLocation("Patel Nagar");
    graph.addLocation("Ballupur");
    graph.addLocation("Clock Tower");
    graph.addLocation("Dalanwala");
    graph.addLocation("Nehru Colony");
    graph.addLocation("Rajpur Road");
    graph.addLocation("Mussoorie Road");

    // Roads with sample distances in kilometres
    graph.addRoad("ISBT Dehradun", "Patel Nagar", 3);
    graph.addRoad("Patel Nagar", "Ballupur", 4);
    graph.addRoad("Ballupur", "Clock Tower", 4);
    graph.addRoad("Clock Tower", "Dalanwala", 3);
    graph.addRoad("Clock Tower", "Nehru Colony", 5);
    graph.addRoad("Dalanwala", "Rajpur Road", 4);
    graph.addRoad("Rajpur Road", "Mussoorie Road", 5);
    graph.addRoad("Ballupur", "Mussoorie Road", 6);
    graph.addRoad("Nehru Colony", "Dalanwala", 4);
}

/* =====================================================
   Display a note about sample data
   ===================================================== */

void EmergencyManager::printSampleDataNote() const
{
    cout << "\nNOTE: Emergency facilities are predefined sample data.\n";
    cout << "Phone numbers are demonstration values, not real contacts.\n";
    cout << "Road distances are illustrative and not verified.\n";
}


// =====================================================
// Files
// =====================================================
string EmergencyManager::contactFileName() const
{
    return "contacts_" + currentUser + ".txt";
}

void EmergencyManager::loadUsers()
{
    ifstream in(USERS_FILE.c_str());
    if (!in) return;

    string line;
    while (getline(in, line))
    {
        vector<string> p = splitLine(line, 4);
        if (p.size() != 4) continue;
        users.push_back(User(p[0], p[1], p[2], p[3]));
    }
}

void EmergencyManager::saveUsers() const
{
    ofstream out(USERS_FILE.c_str());
    for (size_t i = 0; i < users.size(); i++)
        out << users[i].toFileString() << endl;
}

void EmergencyManager::loadContacts()
{
    contacts.clear();
    ifstream in(contactFileName().c_str());
    if (!in) return;

    string line;
    while (getline(in, line))
    {
        vector<string> p = splitLine(line, 3);
        if (p.size() != 3) continue;
        contacts.push_back(Contact(p[0], p[1], p[2]));
    }
}

void EmergencyManager::saveContacts() const
{
    ofstream out(contactFileName().c_str());
    for (size_t i = 0; i < contacts.size(); i++)
        out << contacts[i].toFileString() << endl;
}

// =====================================================
// Account: sign up / sign in / logout
// =====================================================
int EmergencyManager::findUser(const string& username) const
{
    for (size_t i = 0; i < users.size(); i++)      // linear search
    {
        if (users[i].getUsername() == username) return i;
    }
    return -1;
}

void EmergencyManager::signUp()
{
    string name, username, email, password;

    cout << "\n--- SIGN UP ---\n";
    cout << "Enter name: ";
    readText(name);

    while (true)
    {
        cout << "Enter username (3-20 letters/digits/underscore): ";
        readLine(username);
        username = toLowerCase(username);          // usernames are not case-sensitive
        if (!isValidUsername(username))
            cout << "Invalid username. Use only letters, digits and underscore (3-20 characters).\n";
        else if (findUser(username) != -1)
            cout << "Username already taken. Try another.\n";
        else
            break;
    }

    while (true)
    {
        cout << "Enter email: ";
        readLine(email);
        if (isValidEmail(email)) break;
        cout << "Invalid email. Example: student@gmail.com\n";
    }

    while (true)
    {
        cout << "Enter password (minimum 8 characters): ";
        readLine(password);
        if (isValidPassword(password)) break;
        cout << "Password too short. It must have at least 8 characters.\n";
    }

    users.push_back(User(name, username, email, password));
    saveUsers();
    cout << "Sign up successful! You can now sign in.\n";
}

bool EmergencyManager::signIn()
{
    string username, password;

    cout << "\n--- SIGN IN ---\n";
    cout << "Enter username: ";
    readLine(username);
    username = toLowerCase(username);
    cout << "Enter password: ";
    readLine(password);

    int index = findUser(username);
    if (index == -1 || !users[index].checkPassword(password))
    {
        cout << "Wrong username or password.\n";
        return false;
    }

    loggedIn = true;
    currentUser = username;
    loadContacts();                 // this user's contacts
    rebuildQueue();                 // this user's pending SOS go back into the priority queue
    while (!undoStack.empty()) undoStack.pop();

    cout << "Welcome, " << users[index].getName() << "!\n";
    return true;
}

void EmergencyManager::logout()
{
    loggedIn = false;
    currentUser = "";
    contacts.clear();
    while (!sosQueue.empty()) sosQueue.pop();
    while (!undoStack.empty()) undoStack.pop();
    cout << "Logged out successfully.\n";
}

bool EmergencyManager::isLoggedIn() const
{
    return loggedIn;
}

string EmergencyManager::getCurrentName() const
{
    int index = findUser(currentUser);
    if (index == -1) return "";
    return users[index].getName();
}

// =====================================================
// Contacts (vector)
// =====================================================
void EmergencyManager::addContact()
{
    string name, phone, relation;

    cout << "\n--- ADD EMERGENCY CONTACT ---\n";
    cout << "Enter contact name: ";
    readText(name);

    while (true)
    {
        cout << "Enter phone number (10 digits): ";
        readLine(phone);
        if (isValidPhone(phone)) break;
        cout << "Invalid phone number. Enter exactly 10 digits (numbers only).\n";
    }

    cout << "Enter relation (e.g. Father, Friend): ";
    readText(relation);

    contacts.push_back(Contact(name, phone, relation));
    saveContacts();
    cout << "Contact added successfully.\n";
}

void EmergencyManager::viewContacts() const
{
    cout << "\n--- EMERGENCY CONTACTS ---\n";
    if (contacts.empty())
    {
        cout << "No emergency contacts found.\n";
        return;
    }

    cout << left << setw(20) << "Name" << setw(14) << "Phone" << "Relation" << endl;
    cout << "------------------------------------------\n";
    for (size_t i = 0; i < contacts.size(); i++)
        contacts[i].display();
}

// Search by name: case-insensitive, partial match allowed ("ra" finds "Rahul"). Linear search O(n).
void EmergencyManager::searchContact() const
{
    cout << "\n--- SEARCH CONTACT ---\n";
    if (contacts.empty())
    {
        cout << "No emergency contacts found.\n";
        return;
    }

    string name;
    cout << "Enter name to search: ";
    readLine(name);
    string key = toLowerCase(name);

    int found = 0;
    for (size_t i = 0; i < contacts.size(); i++)
    {
        if (toLowerCase(contacts[i].getName()).find(key) != string::npos)
        {
            if (found == 0)
                cout << left << setw(20) << "Name" << setw(14) << "Phone" << "Relation" << endl;
            contacts[i].display();
            found++;
        }
    }
    if (found == 0) cout << "Contact Not Found.\n";
}

// Exact name match (ignoring case). Returns index or -1.
int EmergencyManager::findContactIndex(const string& name) const
{
    string key = toLowerCase(name);
    for (size_t i = 0; i < contacts.size(); i++)
    {
        if (toLowerCase(contacts[i].getName()) == key) return i;
    }
    return -1;
}

// Delete steps: 1) find index  2) shift every later element one place left  3) remove last slot
void EmergencyManager::deleteContact()
{
    cout << "\n--- DELETE CONTACT ---\n";
    if (contacts.empty())
    {
        cout << "No emergency contacts found.\n";
        return;
    }

    string name;
    cout << "Enter exact name of the contact to delete: ";
    readLine(name);

    int index = findContactIndex(name);
    if (index == -1)
    {
        cout << "Contact Not Found.\n";
        return;
    }

    int size = contacts.size();
    for (int i = index; i < size - 1; i++)
        contacts[i] = contacts[i + 1];          // shift left
    contacts.pop_back();                        // size decreases by 1

    saveContacts();
    cout << "Contact deleted successfully.\n";
}

// =====================================================
// SOS
// =====================================================

// Puts all PENDING SOS of the current user into the priority queue.
void EmergencyManager::rebuildQueue()
{
    while (!sosQueue.empty()) sosQueue.pop();

    vector<SOS> mine = history.getRecordsForUser(currentUser);
    for (size_t i = 0; i < mine.size(); i++)
    {
        if (mine[i].getStatus() == "Pending") sosQueue.push(mine[i]);
    }
}

void EmergencyManager::createSOS()
{
    const string types[5] = {"Medical Emergency", "Accident", "Unsafe Situation", "Lost / Stranded", "Other"};
    const string priorities[4] = {"Critical", "High", "Medium", "Low"};

    cout << "\n--- CREATE SOS ---\n";
    cout << "Emergency types:\n";
    for (int i = 0; i < 5; i++)
        cout << "  " << (i + 1) << ". " << types[i] << endl;
    int t = readNumber("Choose emergency type (1-5): ", 1, 5);

    string location;
    cout << "Enter your current location: ";
    readText(location);

    // priority: keep asking until the user enters 1-4
    cout << "Priority levels:\n  1. Critical\n  2. High\n  3. Medium\n  4. Low\n";
    string choice;
    int p = 0;
    while (p == 0)
    {
        cout << "Choose priority (1-4): ";
        readLine(choice);
        if (choice == "1") p = 1;
        else if (choice == "2") p = 2;
        else if (choice == "3") p = 3;
        else if (choice == "4") p = 4;
        else cout << "Invalid priority. Please try again.\n";
    }

    SOS newSOS(nextSosId, currentUser, types[t - 1], location, priorities[p - 1], "Pending");
    nextSosId++;

    history.addRecord(newSOS);          // linked list (permanent record)
    sosQueue.push(newSOS);              // priority queue (waiting to be processed)
    undoStack.push(newSOS.getId());     // stack (so the last SOS can be undone)
    history.saveToFile(HISTORY_FILE);

    cout << "\nSOS created successfully!\n";
    cout << "SOS ID   : " << newSOS.getId() << endl;
    cout << "Type     : " << newSOS.getEmergencyType() << endl;
    cout << "Location : " << newSOS.getLocation() << endl;
    cout << "Priority : " << newSOS.getPriority() << endl;
    cout << "Status   : " << newSOS.getStatus() << endl;
    cout << "(This is a project simulation. No real alert is sent. In a real emergency call 112.)\n";
}

// The priority queue gives the most urgent SOS first (see CompareSOS in SOS.h).
void EmergencyManager::processNextSOS()
{
    cout << "\n--- PROCESS NEXT SOS ---\n";
    if (sosQueue.empty())
    {
        cout << "No emergency requests available.\n";
        return;
    }

    SOS next = sosQueue.top();   // highest priority
    sosQueue.pop();              // remove it from the queue

    SOS* record = history.findById(next.getId(), currentUser);
    if (record != NULL) record->setStatus("Processed");   // update the history record
    history.saveToFile(HISTORY_FILE);

    cout << "Processing the most urgent SOS:\n";
    printSOSHeader();
    next.displayRow();
    cout << "SOS " << next.getId() << " marked as Processed and removed from the queue.\n";
    cout << "Pending SOS left: " << sosQueue.size() << endl;
}

void EmergencyManager::viewSOSHistory() const
{
    cout << "\n--- SOS HISTORY ---\n";
    int count = history.displayForUser(currentUser);
    if (count == 0) cout << "No emergency requests available.\n";
}

void EmergencyManager::searchSOS()
{
    cout << "\n--- SEARCH SOS ---\n";
    if (history.getRecordsForUser(currentUser).empty())
    {
        cout << "No emergency requests available.\n";
        return;
    }

    cout << "1. Search by SOS ID\n2. Search by Status (Pending / Processed / Cancelled)\n";
    int choice = readNumber("Enter choice (1-2): ", 1, 2);

    if (choice == 1)
    {
        int id = readNumber("Enter SOS ID: ", 0, 999999);
        SOS* found = history.findById(id, currentUser);
        if (found == NULL)
        {
            cout << "SOS not found.\n";
        }
        else
        {
            printSOSHeader();
            found->displayRow();
        }
    }
    else
    {
        cout << "1. Pending\n2. Processed\n3. Cancelled\n";
        int s = readNumber("Choose status (1-3): ", 1, 3);
        string status = "Pending";
        if (s == 2) status = "Processed";
        if (s == 3) status = "Cancelled";

        int count = history.searchByStatus(currentUser, status);
        if (count == 0) cout << "No SOS records with status " << status << ".\n";
    }
}

// BUBBLE SORT on a COPY of the records (history and queue are not changed).
// Order: highest priority first; if priorities are equal, smaller ID first.
// Idea: compare neighbours, swap if in wrong order; after each pass the "weakest"
// element sinks to the end. Time: O(n^2) worst/average, O(n) best (already sorted).
void EmergencyManager::sortSOS() const
{
    cout << "\n--- SORT SOS REQUESTS BY PRIORITY ---\n";
    vector<SOS> list = history.getRecordsForUser(currentUser);
    if (list.empty())
    {
        cout << "No emergency requests available.\n";
        return;
    }

    int n = list.size();
    for (int i = 0; i < n - 1; i++)
    {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++)
        {
            bool wrongOrder = false;
            if (list[j].getPriorityLevel() < list[j + 1].getPriorityLevel())
                wrongOrder = true;
            else if (list[j].getPriorityLevel() == list[j + 1].getPriorityLevel() &&
                     list[j].getId() > list[j + 1].getId())
                wrongOrder = true;

            if (wrongOrder)
            {
                SOS temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) break;    // already sorted - stop early
    }

    printSOSHeader();
    for (int i = 0; i < n; i++)
        list[i].displayRow();
}

// Undo = remove the LAST SOS created in this login session (stack: last in, first out).
void EmergencyManager::undoLastSOS()
{
    cout << "\n--- UNDO LAST SOS ---\n";
    if (undoStack.empty())
    {
        cout << "No recent SOS to undo.\n";
        return;
    }

    int id = undoStack.top();
    undoStack.pop();

    SOS* record = history.findById(id, currentUser);
    if (record == NULL || record->getStatus() != "Pending")
    {
        cout << "SOS " << id << " is already processed or cancelled, so it cannot be undone.\n";
        return;
    }

    record->setStatus("Cancelled");     // keep it in history, but mark as cancelled
    rebuildQueue();                     // a priority queue cannot delete from the middle, so rebuild it
    history.saveToFile(HISTORY_FILE);
    cout << "SOS " << id << " cancelled. It was removed from the priority queue.\n";
}

// =====================================================
// Facilities (vector + linear search)
// =====================================================
void EmergencyManager::showFacilityHeader() const
{
    cout << left << setw(5) << "ID" << setw(24) << "Name" << setw(16) << "Type"
         << setw(18) << "Area" << "Phone (demo)" << endl;
    cout << "---------------------------------------------------------------------\n";
}

void EmergencyManager::viewFacilities() const
{
    cout << "\n--- EMERGENCY FACILITIES ---\n";
    printSampleDataNote();
    showFacilityHeader();
    for (size_t i = 0; i < facilities.size(); i++)
        facilities[i].display();
}

void EmergencyManager::searchFacilitiesByType(const string& type)
{
    int found = 0;
    for (size_t i = 0; i < facilities.size(); i++)
    {
        if (facilities[i].getType() == type)
        {
            if (found == 0) showFacilityHeader();
            facilities[i].display();
            found++;
        }
    }
    if (found == 0) cout << "No facilities found.\n";
}

void EmergencyManager::searchFacilities()
{
    cout << "\n--- SEARCH EMERGENCY FACILITIES ---\n";
    printSampleDataNote();
    cout << "1. Search by area\n2. Search hospitals\n3. Search police stations\n";
    int choice = readNumber("Enter choice (1-3): ", 1, 3);

    if (choice == 1)
    {
        string area;
        cout << "Enter area (e.g. Roorkee, Civil Lines, Bus Stand): ";
        readLine(area);
        string key = toLowerCase(area);

        int found = 0;
        for (size_t i = 0; i < facilities.size(); i++)
        {
            if (toLowerCase(facilities[i].getArea()) == key)
            {
                if (found == 0) showFacilityHeader();
                facilities[i].display();
                found++;
            }
        }
        if (found == 0) cout << "No facilities found in this area.\n";
    }
    else if (choice == 2)
    {
        searchFacilitiesByType("Hospital");
    }
    else
    {
        searchFacilitiesByType("Police Station");
    }
}

// =====================================================
// Graph features
// =====================================================
void EmergencyManager::viewMap() const
{
    cout << "\n--- ROAD MAP (GRAPH) ---\n";
    cout << "Sample map used for demonstration. Distances are sample values in km.\n";
    graph.displayGraph();
}

// BFS from a start location. Level = number of roads from the start (NOT km).
void EmergencyManager::nearbyFacilitiesBFS()
{
    cout << "\n--- NEARBY FACILITIES (BFS) ---\n";
    graph.displayLocations();

    string start;
    cout << "Enter starting location: ";
    readText(start);

    int s = graph.findLocation(start);
    if (s == -1)
    {
        cout << "Location not found.\n";
        return;
    }

    vector<int> order, level;
    graph.bfs(s, order, level);

    printSampleDataNote();
    cout << "Locations visited level by level (level = number of roads from start):\n";

    int totalFound = 0;
    for (size_t i = 0; i < order.size(); i++)
    {
        int node = order[i];
        cout << "Level " << level[node] << ": " << graph.getName(node) << endl;

        // print facilities whose area is this location
        for (size_t j = 0; j < facilities.size(); j++)
        {
            if (toLowerCase(facilities[j].getArea()) == toLowerCase(graph.getName(node)))
            {
                cout << "         -> " << facilities[j].getName() << " (" << facilities[j].getType()
                     << ", " << facilities[j].getPhone() << ")\n";
                totalFound++;
            }
        }
    }
    cout << "Total facilities found: " << totalFound << endl;
}

void EmergencyManager::shortestRoute()
{
    cout << "\n--- SHORTEST ROUTE (DIJKSTRA) ---\n";
    graph.displayLocations();

    string from, to;
    cout << "Enter start location: ";
    readText(from);
    cout << "Enter destination: ";
    readText(to);

    int s = graph.findLocation(from);
    int d = graph.findLocation(to);
    if (s == -1 || d == -1)
    {
        cout << "Location not found.\n";
        return;
    }

    int total = 0;
    vector<int> path;
    if (!graph.dijkstra(s, d, total, path))
    {
        cout << "No route found between these locations.\n";
        return;
    }

    cout << "Shortest distance: " << total << " km (sample distance)\n";
    cout << "Route:\n";
    for (size_t i = 0; i < path.size(); i++)
    {
        cout << graph.getName(path[i]);
        if (i + 1 < path.size()) cout << " -> ";
    }
    cout << endl;
}
