// main.cpp - menus only. All real work is done by EmergencyManager.
#include <iostream>
#include "EmergencyManager.h"
#include "Validation.h"
using namespace std;

void showBanner()
{
    cout << "\n====================================\n";
    cout << "        SAFEROUTE SOS\n";
    cout << "====================================\n";
    cout << "Academic prototype - NOT a real emergency service.\n";
    cout << "Locations and facilities are predefined demo data.\n";
    cout << "In a real emergency call 112 (India).\n";
}

void showUserMenu()
{
    cout << "\n====================================\n";
    cout << "        SAFEROUTE MENU\n";
    cout << "====================================\n";
    cout << " 1. Add Emergency Contact\n";
    cout << " 2. View Emergency Contacts\n";
    cout << " 3. Search Contact\n";
    cout << " 4. Delete Emergency Contact\n";
    cout << " 5. Create SOS\n";
    cout << " 6. Process Next SOS\n";
    cout << " 7. View SOS History\n";
    cout << " 8. Search SOS\n";
    cout << " 9. View Emergency Facilities\n";
    cout << "10. Search Emergency Facilities\n";
    cout << "11. Find Nearby Facilities using BFS\n";
    cout << "12. Find Shortest Route using Dijkstra\n";
    cout << "13. Sort SOS Requests\n";
    cout << "14. Undo Last SOS\n";
    cout << "15. View Road Map (Graph)\n";
    cout << "16. Logout\n";
}

// Menu shown after a successful sign in. Returns when the user logs out.
void userSession(EmergencyManager& manager)
{
    int choice = 0;
    while (manager.isLoggedIn())
    {
        showUserMenu();
        choice = readNumber("Enter your choice (1-16): ", 1, 16);

        switch (choice)
        {
        case 1:  manager.addContact(); break;
        case 2:  manager.viewContacts(); break;
        case 3:  manager.searchContact(); break;
        case 4:  manager.deleteContact(); break;
        case 5:  manager.createSOS(); break;
        case 6:  manager.processNextSOS(); break;
        case 7:  manager.viewSOSHistory(); break;
        case 8:  manager.searchSOS(); break;
        case 9:  manager.viewFacilities(); break;
        case 10: manager.searchFacilities(); break;
        case 11: manager.nearbyFacilitiesBFS(); break;
        case 12: manager.shortestRoute(); break;
        case 13: manager.sortSOS(); break;
        case 14: manager.undoLastSOS(); break;
        case 15: manager.viewMap(); break;
        case 16: manager.logout(); break;
        }
    }
}

int main()
{
    EmergencyManager manager;
    showBanner();

    int choice = 0;
    while (choice != 3)
    {
        cout << "\nMAIN MENU\n";
        cout << "1. Sign Up\n";
        cout << "2. Sign In\n";
        cout << "3. Exit\n";
        choice = readNumber("Enter your choice (1-3): ", 1, 3);

        if (choice == 1)
        {
            manager.signUp();
        }
        else if (choice == 2)
        {
            if (manager.signIn())
                userSession(manager);
        }
    }

    cout << "Thank you for using SafeRoute SOS. Stay safe!\n";
    return 0;
}

