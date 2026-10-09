#include <iostream>
#include <string>
#include <regex>
using namespace std;

// Check email format
bool validEmail(string email)
{
    regex pattern(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    return regex_match(email, pattern);
}

// Check phone number: exactly 10 digits
bool validPhone(string phone)
{
    if (phone.length() != 10)
        return false;

    for (int i = 0; i < phone.length(); i++)
    {
        if (phone[i] < '0' || phone[i] > '9')
            return false;
    }

    return true;
}

// User class
class User
{
private:
    string name;
    string username;
    string email;
    string password;

public:

    // Sign up
    void signUp()
    {
        cout << "\n===== SIGN UP =====\n";

        cout << "Enter Name: ";
        getline(cin >> ws, name);

        cout << "Enter Username: ";
        getline(cin >> ws, username);

        // Validate email
        do
        {
            cout << "Enter Email: ";
            getline(cin >> ws, email);

            if (!validEmail(email))
                cout << "Invalid email! Enter a valid email address.\n";

        } while (!validEmail(email));

        // Password must contain at least 8 characters
        do
        {
            cout << "Create Password (minimum 8 characters): ";
            getline(cin >> ws, password);

            if (password.length() < 8)
                cout << "Password must be at least 8 characters long!\n";

        } while (password.length() < 8);

        cout << "\nAccount Created Successfully!\n";
    }

    // Sign in
    bool signIn()
    {
        string enteredUsername;
        string enteredPassword;

        cout << "\n===== SIGN IN =====\n";

        cout << "Enter Username: ";
        getline(cin >> ws, enteredUsername);

        cout << "Enter Password: ";
        getline(cin >> ws, enteredPassword);

        if (enteredUsername == username &&
            enteredPassword == password)
        {
            cout << "\nLogin Successful!\n";
            cout << "Welcome, " << name << "!\n";
            return true;
        }

        cout << "\nInvalid Username or Password!\n";
        return false;
    }
};


// Contact class
class Contact
{
private:
    string name;
    string phone;
    string relation;

public:

    // Add emergency contact
    void addContact()
    {
        cout << "\n===== ADD EMERGENCY CONTACT =====\n";

        cout << "Enter Contact Name: ";
        getline(cin >> ws, name);

        // Validate phone number
        do
        {
            cout << "Enter 10-digit Phone Number: ";
            getline(cin >> ws, phone);

            if (!validPhone(phone))
                cout << "Invalid phone number! Enter exactly 10 digits.\n";

        } while (!validPhone(phone));

        cout << "Enter Relation: ";
        getline(cin >> ws, relation);

        cout << "\nContact Added Successfully!\n";
    }

    // Display contact
    void display()
    {
        cout << "Name     : " << name << endl;
        cout << "Phone    : " << phone << endl;
        cout << "Relation : " << relation << endl;
    }

    // Get contact name
    string getName()
    {
        return name;
    }
};


// SOS class
class SOS
{
public:

    void createSOS()
    {
        string emergencyType;
        string location;

        cout << "\n===== CREATE SOS =====\n";

        cout << "Enter Emergency Type: ";
        getline(cin >> ws, emergencyType);

        cout << "Enter Location: ";
        getline(cin >> ws, location);

        cout << "\nSOS Created Successfully!\n";
        cout << "Emergency : " << emergencyType << endl;
        cout << "Location  : " << location << endl;
    }
};


int main()
{
    User user;
    SOS sos;

    Contact contacts[5];
    int contactCount = 0;

    string choice;

    cout << "\n================================\n";
    cout << "          SAFEROUTE SOS\n";
    cout << "================================\n";

    while (true)
    {
        cout << "\n===== MAIN MENU =====\n";
        cout << "1. Sign Up\n";
        cout << "2. Sign In\n";
        cout << "3. Exit\n";

        cout << "Enter choice: ";
        getline(cin >> ws, choice);

        if (choice == "1")
        {
            user.signUp();
        }
        else if (choice == "2")
        {
            if (user.signIn())
            {
                string option;

                while (true)
                {
                    cout << "\n===== SAFEROUTE MENU =====\n";
                    cout << "1. Add Emergency Contact\n";
                    cout << "2. View Emergency Contacts\n";
                    cout << "3. Delete Emergency Contact\n";
                    cout << "4. Create SOS\n";
                    cout << "5. Logout\n";

                    cout << "Enter choice: ";
                    getline(cin >> ws, option);

                    // Add contact
                    if (option == "1")
                    {
                        if (contactCount < 5)
                        {
                            contacts[contactCount].addContact();
                            contactCount++;
                        }
                        else
                        {
                            cout << "\nMaximum 5 contacts allowed!\n";
                        }
                    }

                    // View contacts
                    else if (option == "2")
                    {
                        if (contactCount == 0)
                        {
                            cout << "\nNo emergency contacts found.\n";
                        }
                        else
                        {
                            for (int i = 0; i < contactCount; i++)
                            {
                                cout << "\nContact " << i + 1 << endl;
                                contacts[i].display();
                            }
                        }
                    }

                    // Delete contact
                    else if (option == "3")
                    {
                        if (contactCount == 0)
                        {
                            cout << "\nNo contacts to delete.\n";
                        }
                        else
                        {
                            string deleteName;
                            bool found = false;

                            cout << "Enter Contact Name to Delete: ";
                            getline(cin >> ws, deleteName);

                            for (int i = 0; i < contactCount; i++)
                            {
                                if (contacts[i].getName() == deleteName)
                                {
                                    for (int j = i; j < contactCount - 1; j++)
                                    {
                                        contacts[j] = contacts[j + 1];
                                    }

                                    contactCount--;
                                    found = true;

                                    cout << "\nContact Deleted Successfully!\n";
                                    break;
                                }
                            }

                            if (!found)
                                cout << "\nContact Not Found!\n";
                        }
                    }

                    // Create SOS
                    else if (option == "4")
                    {
                        sos.createSOS();
                    }

                    // Logout
                    else if (option == "5")
                    {
                        cout << "\nLogged out successfully!\n";
                        break;
                    }
                    else
                    {
                        cout << "\nInvalid choice!\n";
                    }
                }
            }
        }
        else if (choice == "3")
        {
            cout << "\nThank you for using SafeRoute SOS!\n";
            break;
        }
        else
        {
            cout << "\nInvalid choice!\n";
        }
    }

    return 0;
}