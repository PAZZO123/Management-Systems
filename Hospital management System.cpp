#include <iostream>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

struct Patient {
    int id;
    string name;
    float age;
    string disease;
    Patient* next;
};

class PatientRecordSystem {
private:
    Patient* head;

public:
    PatientRecordSystem() {
        head = nullptr;
    }

    ~PatientRecordSystem() {
        while (head != nullptr) {
            Patient* temp = head;
            head = head->next;
            delete temp;
        }
    }

    bool idExists(int id) {
        Patient* temp = head;
        while (temp != nullptr) {
            if (temp->id == id) return true;
            temp = temp->next;
        }
        return false;
    }

    bool isValidName(const string& name) {
        for (char c : name) {
            if (!isalpha(c) && c != ' ' && c != '-') {
                return false;
            }
        }
        return !name.empty();
    }

    void addPatient(int id, string name, float age, string disease) {
        if (idExists(id)) {
            cout << "Error: Patient with ID " << id << " already exists!\n";
            return;
        }

        if (age <= 0.0 || age > 150.0) {
            cout << "Error: Age must be between 1 and 150!\n";
            return;
        }

        if (!isValidName(name)) {
            cout << "Error: Invalid name! Name must only contain letters, spaces, or hyphens.\n";
            return;
        }

        Patient* newPatient = new Patient{id, name, age, disease, nullptr};

        if (head == nullptr) {
            head = newPatient;
        } else {
            Patient* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newPatient;
        }
        cout << "Patient record added successfully!\n";
    }

    void deletePatient(int id) {
        if (head == nullptr) {
            cout << "No records found!\n";
            return;
        }

        Patient* temp = head;
        Patient* prev = nullptr;

        if (temp != nullptr && temp->id == id) {
            head = temp->next;
            delete temp;
            cout << "Patient record deleted successfully!\n";
            return;
        }

        while (temp != nullptr && temp->id != id) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == nullptr) {
            cout << "Patient with ID " << id << " not found!\n";
            return;
        }

        prev->next = temp->next;
        delete temp;
        cout << "Patient record deleted successfully!\n";
    }

    void updatePatient(int id) {
        Patient* temp = head;
        while (temp != nullptr) {
            if (temp->id == id) {
                string name;
                int age;
                string disease;

                cout << "Enter new name (Current: " << temp->name << "): ";
                getline(cin, name);
                cout << "Enter new age (Current: " << temp->age << "): ";
                cin >> age;
                cin.ignore(); 
                cout << "Enter new disease (Current: " << temp->disease << "): ";
                getline(cin, disease);

                if (age <= 0 || age > 150) {
                    cout << "Error: Age must be between 1 and 150!\n";
                    return;
                }

                if (!isValidName(name)) {
                    cout << "Error: Invalid name! Name must only contain letters, spaces, or hyphens.\n";
                    return;
                }

                temp->name = name;
                temp->age = age;
                temp->disease = disease;
                cout << "Patient record updated successfully!\n";
                return;
            }
            temp = temp->next;
        }

        cout << "Patient with ID " << id << " not found!\n";
    }

    void displayPatients() {
        if (head == nullptr) {
            cout << "No records available!\n";
            return;
        }

        Patient* temp = head;
        int count = 0;
        cout << "\n--- Patient Records ---\n";
        while (temp != nullptr) {
            cout << "ID: " << temp->id << ", Name: " << temp->name
                 << ", Age: " << temp->age << ", Disease: " << temp->disease << "\n";
            temp = temp->next;
            count++;
        }
        cout << "Total Patients: " << count << "\n";
    }
};

int main() {
    PatientRecordSystem hospital;
    int choice, id;
	float age;
    string name, disease;

    do {
        cout << "\nHospital Patient Record System\n";
        cout << "1. Add Patient\n2. Delete Patient\n3. Update Patient\n4. Display Patients\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        
        cin.ignore();  // Ignore leftover newline

        switch (choice) {
            case 1:
                cout << "Enter Patient ID: ";
                cin >> id;
                if (cin.fail() || id <= 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid ID! Please enter a positive integer.\n";
                    break;
                }
                cin.ignore();
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter Age: ";
                cin >> age;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid age! Please enter a number.\n";
                    break;
                }
                cin.ignore();
                cout << "Enter Disease: ";
                getline(cin, disease);
                hospital.addPatient(id, name, age, disease);
                break;

            case 2:
                cout << "Enter Patient ID to delete: ";
                cin >> id;
                if (cin.fail() || id <= 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid ID! Please enter a positive integer.\n";
                    break;
                }
                hospital.deletePatient(id);
                break;

            case 3:
                cout << "Enter Patient ID to update: ";
                cin >> id;
                if (cin.fail() || id <= 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid ID! Please enter a positive integer.\n";
                    break;
                }
                cin.ignore(); // Ignore leftover newline
                hospital.updatePatient(id);
                break;

            case 4:
                hospital.displayPatients();
                break;

            case 5:
                cout << "Exiting system...\n";
                break;

            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}

