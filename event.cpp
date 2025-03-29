#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <regex>
#include <vector>

using namespace std;

// Structure to store event details
struct Event
{
    string name;
    string date;
    string venue;
    int priority;
    vector<string> participants; // List of participants
    Event *next;
};
 
// Class for Event Management
class EventManager
{
private:
    Event *head;

    // Function to validate date format
    bool isValidDate(const string &date)
    {
        regex datePattern(R"(^\d{2}/\d{2}/\d{4}$)"); // DD/MM/YYYY
        if (!regex_match(date, datePattern))
        {
            return false;
        }

        int day = stoi(date.substr(0, 2));
        int month = stoi(date.substr(3, 2));
        int year = stoi(date.substr(6, 4));

        if (month < 1 || month > 12 || day < 1 || year < 1900)
        {
            return false;
        }

        // Number of days in each month
        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        // Leap year check
        if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
        {
            daysInMonth[1] = 29;
        }

        return day <= daysInMonth[month - 1];
    }

    // Function to validate string input
    string getValidStringInput(string prompt)
    {
        string input;
        while (true)
        {
            cout << prompt;
            getline(cin, input);
            if (!input.empty())
            {
                return input;
            }
            cout << "Invalid input. Please enter a valid value.\n";
        }
    }

    // Function to validate integer input
    int getValidIntInput(string prompt)
    {
        int value;
        while (true)
        {
            cout << prompt;
            if (cin >> value && value > 0)
            {
                cin.ignore();
                return value;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a positive number.\n";
        }
    }

public:
    EventManager()
    {
        head = nullptr;
        loadFromFile();
    }

    void addEvent()
    {
        string name = getValidStringInput("\nEnter event name: ");
        string date;
        while (true)
        {
            date = getValidStringInput("Enter event date (DD/MM/YYYY): ");
            if (isValidDate(date))
                break;
            cout << "Invalid date format. Please enter a valid date in DD/MM/YYYY format.\n";
        }

        string venue = getValidStringInput("Enter event venue: ");
        int priority = getValidIntInput("Enter event priority (higher number = higher priority): ");
        while (!(cin >> priority))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\nPlease enter a number: ";
        }
        cin.ignore();

        // Input participants
        vector<string> participants;
        cout << "Enter participants (type 'done' to finish):\n";
        while (true)
        {
            string participant = getValidStringInput("Participant name: ");
            if (participant == "done")
                break;
            participants.push_back(participant);
        }

        Event *newEvent = new Event{name, date, venue, priority, participants, nullptr};

        if (!head || priority > head->priority)
        {
            newEvent->next = head;
            head = newEvent;
        }
        else
        {
            Event *temp = head;
            while (temp->next && temp->next->priority >= priority)
            {
                temp = temp->next;
            }
            newEvent->next = temp->next;
            temp->next = newEvent;
        }

        cout << "Event added successfully!\n";
        saveToFile();
    }

    // Function to display all events
    void displayEvents()
    {
        if (!head)
        {
            cout << "No events scheduled.\n";
            return;
        }

        Event *temp = head;
        cout << "\n";
        cout << "\nUpcoming Events:\n";
        while (temp)
        {
            cout << "\n";
            cout << "Event: " << temp->name
                 << " | Date: " << temp->date
                 << " | Venue: " << temp->venue
                 << " | Priority: " << temp->priority << endl;
            cout << "Participants: ";
            for (const string &participant : temp->participants)
            {
                cout << participant << ", ";
            }
            cout << endl;
            temp = temp->next;
        }
    }

    void removeEvent()
    {
        string name = getValidStringInput("Enter event name to remove: ");
        if (!head)
        {
            cout << "No events to remove.\n";
            return;
        }

        if (head->name == name)
        {
            Event *temp = head;
            head = head->next;
            delete temp;
            cout << "Event removed successfully.\n";
            saveToFile();
            return;
        }

        Event *temp = head;
        while (temp->next && temp->next->name != name)
        {
            temp = temp->next;
        }

        if (temp->next)
        {
            Event *toDelete = temp->next;
            temp->next = temp->next->next;
            delete toDelete;
            cout << "Event removed successfully.\n";
            saveToFile();
        }
        else
        {
            cout << "Event not found.\n";
        }
    }

    void searchEvent()
    {
        string name = getValidStringInput("Enter event name to search: ");
        Event *temp = head;
        while (temp)
        {
            if (temp->name == name)
            {
                cout << "\nEvent Found!\n";
                cout << "Event: " << temp->name
                     << " | Date: " << temp->date
                     << " | Venue: " << temp->venue
                     << " | Priority: " << temp->priority << endl;
                cout << "Participants: ";
                for (const string &participant : temp->participants)
                {
                    cout << participant << ", ";
                }
                cout << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "Event not found.\n";
    }

    void updateEvent()
    {
        string name = getValidStringInput("Enter event name to update: ");
        Event *temp = head;
        while (temp)
        {
            if (temp->name == name)
            {
                cout << "\nEnter new details for the event:\n";
                temp->name = getValidStringInput("New Event Name: ");
                while (true)
                {
                    temp->date = getValidStringInput("New Event Date (DD/MM/YYYY): ");
                    if (isValidDate(temp->date))
                        break;
                    cout << "Invalid date format. Please enter a valid date.\n";
                }
                temp->venue = getValidStringInput("New Event Venue: ");
                temp->priority = getValidIntInput("New Event Priority: ");

                // Update participants
                temp->participants.clear();
                cout << "Enter new participants (type 'done' to finish):\n";
                while (true)
                {
                    string participant = getValidStringInput("Participant name: ");
                    if (participant == "done")
                        break;
                    temp->participants.push_back(participant);
                }

                cout << "Event updated successfully!\n";
                saveToFile();
                return;
            }
            temp = temp->next;
        }
        cout << "Event not found.\n";
    }

    // Load events from a file
    void loadFromFile()
    {
        ifstream file("events.txt");
        if (!file)
            return;

        string name, date, venue;
        int priority;
        while (getline(file, name) && getline(file, date) && getline(file, venue) && (file >> priority))
        {
            file.ignore();
            vector<string> participants;
            string participant;
            while (getline(file, participant) && participant != "====================")
            {
                if (participant.find("Participant: ") == 0)
                {
                    participants.push_back(participant.substr(12));
                }
            }
            Event *newEvent = new Event{name, date, venue, priority, participants, nullptr};
            newEvent->next = head;
            head = newEvent;
        }
        file.close();
    }

    // Save events to a file
    void saveToFile()
    {
        ofstream file("events.txt");

        // Initialize the counter to count the number of events
        int n = 0;
        Event *temp = head;

        // Count the number of events in the list
        while (temp)
        {
            n++; // Increment for each event
            temp = temp->next;
        }

        // Write each event with RECORD 1, RECORD 2 format
        temp = head;
        int recordNumber = 1;
        while (temp)
        {
            file << "RECORD " << recordNumber << "\n";
            file << "====================\n";
            file << "Event: " << temp->name << "\n"
                 << "Date: " << temp->date << "\n"
                 << "Venue: " << temp->venue << "\n"
                 << "Priority: " << temp->priority << "\n";
            for (const string &participant : temp->participants)
            {
                file << "Participant: " << participant << "\n";
            }
            file << "====================\n";

            temp = temp->next;
            recordNumber++; // Increment the record number
        }

        // Add the total number of records at the end of the file
        file << "Total Records: " << n << "\n";

        file.close();
    }

    ~EventManager()
    {
        while (head)
        {
            Event *temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// Main function
int main()
{
    EventManager manager;
    int choice;

    do
    {
        cout << "\n1. Add Event\n2. Display Events\n3. Search Event\n4. Update Event\n5. Remove Event\n6. Exit\nEnter choice: ";
        cin >> choice;
        /*
        while (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\nPlease enter a number: ";
        }
        cin.ignore();
        */
        switch (choice)
        {
        case 1:
            manager.addEvent();
            break;
        case 2:
            manager.displayEvents();
            break;
        case 3:
            manager.searchEvent();
            break;
        case 4:
            manager.updateEvent();
            break;
        case 5:
            manager.removeEvent();
            break;
        case 6:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 6);

    return 0;
}