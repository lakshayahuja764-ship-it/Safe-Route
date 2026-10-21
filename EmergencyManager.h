// EmergencyManager.h - the "brain" of the project.
// It owns all the data structures and does the work for every menu option.
#ifndef EMERGENCYMANAGER_H
#define EMERGENCYMANAGER_H

#include <string>
#include <vector>
#include <queue>
#include <stack>
#include "User.h"
#include "Contact.h"
#include "SOS.h"
#include "Facility.h"
#include "Graph.h"
using namespace std;

class EmergencyManager
{
private:
    // ----- accounts -----
    vector<User> users;
    bool loggedIn;
    string currentUser;

    // ----- contacts (vector; delete shifts elements manually) -----
    vector<Contact> contacts;

    // ----- SOS data structures -----
    SOSHistory history;                                          // linked list of all SOS records
    priority_queue<SOS, vector<SOS>, CompareSOS> sosQueue;       // pending SOS, most urgent on top
    stack<int> undoStack;                                        // IDs of SOS created this login (LIFO)
    int nextSosId;

    // ----- facilities and map -----
    vector<Facility> facilities;
    Graph graph;

    // ----- private helpers -----
    int findUser(const string& username) const;
    string contactFileName() const;
    void loadUsers();
    void saveUsers() const;
    void loadContacts();
    void saveContacts() const;
    void loadSampleFacilities();
    void loadSampleGraph();
    void rebuildQueue();
    int findContactIndex(const string& name) const;
    void showFacilityHeader() const;
    void searchFacilitiesByType(const string& type);
    void printSampleDataNote() const;

public:
    EmergencyManager();

    // account
    void signUp();
    bool signIn();
    void logout();
    bool isLoggedIn() const;
    string getCurrentName() const;

    // contacts
    void addContact();
    void viewContacts() const;
    void searchContact() const;
    void deleteContact();

    // SOS
    void createSOS();
    void processNextSOS();
    void viewSOSHistory() const;
    void searchSOS();
    void sortSOS() const;
    void undoLastSOS();

    // facilities and graph
    void viewFacilities() const;
    void searchFacilities();
    void nearbyFacilitiesBFS();
    void shortestRoute();
    void viewMap() const;
};

#endif
