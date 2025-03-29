#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Structure to store student information
struct Student {
    int id;
    string firstName;
    string lastName;
    float grade;
    string status;  // Active, Inactive, etc.
    Student* next;
};

class StudentManager {
private:
    Student* head;
    Student* studentArray[100];
    int count;

public:
    StudentManager() : head(nullptr), count(0) {}

    // Function to check if student ID already exists in the array
    bool studentExistsInArray(int id) {
        for (int i = 0; i < count; i++) {
            if (studentArray[i]->id == id) {
                return true;
            }
        }
        return false;
    }

    // Function to check if student ID already exists in the linked list
    bool studentExistsInLinkedList(int id) {
        Student* temp = head;
        while (temp != nullptr) {
            if (temp->id == id) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    // Function to add student to array
    void addStudentToArray() {
        if (count >= 100) {
            cout << "Array is full. Cannot add more students.\n";
            return;
        }

        int id;
        cout << "Enter Student ID: ";
        cin >> id;

        if (studentExistsInArray(id)) {
            cout << "Error: Student ID already exists in the array. Please use a different ID.\n";
            return;
        }

        Student* newStudent = new Student;
        newStudent->id = id;
        cout << "Enter First Name: ";
        cin >> newStudent->firstName;
        cout << "Enter Last Name: ";
        cin >> newStudent->lastName;
        cout << "Enter Grade: ";
        cin >> newStudent->grade;
        cout << "Enter Status (Active/Inactive): ";
        cin >> newStudent->status;

        studentArray[count] = newStudent;
        count++;
        cout << "Student added to array.\n";
    }

    // Function to add student to linked list
    void addStudentToLinkedList() {
        int id;
        cout << "Enter Student ID: ";
        cin >> id;

        if (studentExistsInLinkedList(id)) {
            cout << "Error: Student ID already exists in the linked list. Please use a different ID.\n";
            return;
        }

        Student* newStudent = new Student;
        newStudent->id = id;
        cout << "Enter First Name: ";
        cin >> newStudent->firstName;
        cout << "Enter Last Name: ";
        cin >> newStudent->lastName;
        cout << "Enter Grade: ";
        cin >> newStudent->grade;
        cout << "Enter Status (Active/Inactive): ";
        cin >> newStudent->status;

        newStudent->next = head;
        head = newStudent;
        cout << "Student added to linked list.\n";
    }

    // Function to display all students in the array
    void displayStudentsArray() {
        if (count == 0) {
            cout << "Empty in Array.\n";
            return;
        }

        cout << "Displaying Students (Array):\n";
        for (int i = 0; i < count; i++) {
            cout << "ID: " << studentArray[i]->id
                 << ", Name: " << studentArray[i]->firstName << " " << studentArray[i]->lastName
                 << ", Grade: " << studentArray[i]->grade
                 << ", Status: " << studentArray[i]->status << endl;
        }
    }

    // Function to display all students in the linked list
    void displayStudentsLinkedList() {
        if (head == nullptr) {
            cout << "Empty in Linked List.\n";
            return;
        }

        cout << "Displaying Students (Linked List):\n";
        Student* temp = head;
        while (temp != nullptr) {
            cout << "ID: " << temp->id
                 << ", Name: " << temp->firstName << " " << temp->lastName
                 << ", Grade: " << temp->grade
                 << ", Status: " << temp->status << endl;
            temp = temp->next;
        }
    }

    // Function to delete a student by ID from the array
    void deleteStudentFromArray() {
        int id;
        cout << "Enter Student ID to delete: ";
        cin >> id;

        bool found = false;
        for (int i = 0; i < count; i++) {
            if (studentArray[i]->id == id) {
                delete studentArray[i];  // Free memory
                for (int j = i; j < count - 1; j++) {
                    studentArray[j] = studentArray[j + 1];  // Shift the array left
                }
                studentArray[count - 1] = nullptr;  // Reset last element
                count--;
                cout << "Student deleted from array.\n";
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Student with ID " << id << " not found in array.\n";
        }
    }

    // Function to delete a student by ID from the linked list
    void deleteStudentFromLinkedList() {
        int id;
        cout << "Enter Student ID to delete: ";
        cin >> id;

        Student* temp = head;
        Student* prev = nullptr;

        // If the student to delete is the head
        if (temp != nullptr && temp->id == id) {
            head = temp->next;  // Move head to the next student
            delete temp;
            cout << "Student deleted from linked list.\n";
            return;
        }

        // Search for the student to delete
        while (temp != nullptr && temp->id != id) {
            prev = temp;
            temp = temp->next;
        }

        // If student wasn't found
        if (temp == nullptr) {
            cout << "Student with ID " << id << " not found in linked list.\n";
            return;
        }

        // Unlink the node from the linked list
        prev->next = temp->next;
        delete temp;
        cout << "Student deleted from linked list.\n";
    }

    // Function to search for a student by ID in the array
    void searchStudentInArray() {
        int id;
        cout << "Enter Student ID to search: ";
        cin >> id;

        bool found = false;
        for (int i = 0; i < count; i++) {
            if (studentArray[i]->id == id) {
                cout << "Student found: ID: " << studentArray[i]->id
                     << ", Name: " << studentArray[i]->firstName << " " << studentArray[i]->lastName
                     << ", Grade: " << studentArray[i]->grade
                     << ", Status: " << studentArray[i]->status << endl;
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Student with ID " << id << " not found in array.\n";
        }
    }

    // Function to search for a student by ID in the linked list
    void searchStudentInLinkedList() {
        int id;
        cout << "Enter Student ID to search: ";
        cin >> id;

        Student* temp = head;
        bool found = false;

        while (temp != nullptr) {
            if (temp->id == id) {
                cout << "Student found: ID: " << temp->id
                     << ", Name: " << temp->firstName << " " << temp->lastName
                     << ", Grade: " << temp->grade
                     << ", Status: " << temp->status << endl;
                found = true;
                break;
            }
            temp = temp->next;
        }

        if (!found) {
            cout << "Student with ID " << id << " not found in linked list.\n";
        }
    }

    // Function to calculate and display the average grade
    void calculateAverageGrade() {
        if (count == 0) {
            cout << "No students in the array to calculate average grade.\n";
            return;
        }
        float total = 0;
        for (int i = 0; i < count; i++) {
            total += studentArray[i]->grade;
        }
        float average = total / count;
        cout << "Average Grade: " << average << endl;
    }

    // Function to sort students by grade
    void sortStudentsByGrade() {
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (studentArray[i]->grade > studentArray[j]->grade) {
                    // Swap the students
                    Student* temp = studentArray[i];
                    studentArray[i] = studentArray[j];
                    studentArray[j] = temp;
                }
            }
        }
        cout << "Students sorted by grade.\n";
    }

    // Function to save students to a file
    void saveDataToFile() {
        if (count == 0) {
            cout << "No data to save.\n";
            return;
        }

        ofstream file("students.txt");
        for (int i = 0; i < count; i++) {
            file << studentArray[i]->id << " "
                 << studentArray[i]->firstName << " "
                 << studentArray[i]->lastName << " "
                 << studentArray[i]->grade << " "
                 << studentArray[i]->status << endl;
        }
        file.close();
        cout << "Data saved to file.\n";
    }

    // Function to load students from a file
    void loadDataFromFile() {
        ifstream file("students.txt");
        if (!file) {
            cout << "No file found or file is empty.\n";
            return;
        }

        int id;
        string firstName, lastName, status;
        float grade;
        while (file >> id >> firstName >> lastName >> grade >> status) {
            Student* newStudent = new Student;
            newStudent->id = id;
            newStudent->firstName = firstName;
            newStudent->lastName = lastName;
            newStudent->grade = grade;
            newStudent->status = status;
            studentArray[count++] = newStudent;
        }
        file.close();
        cout << "Data loaded from file.\n";
    }

    // Function to quit the system
    void quitSystem() {
        cout << "Exiting the system...\n";
        // Clean up memory from the linked list
        Student* temp;
        while (head != nullptr) {
            temp = head;
            head = head->next;
            delete temp;
        }
        // Clean up memory from the array
        for (int i = 0; i < count; i++) {
            delete studentArray[i];
        }
    }
};

int main() {
    StudentManager manager;
    int choice;
    do {
        cout << "\nStudent Record Management System\n";
        cout << "1. Add Student to Array\n";
        cout << "2. Add Student to Linked List\n";
        cout << "3. Display Students (Array)\n";
        cout << "4. Display Students (Linked List)\n";
        cout << "5. Delete Student from Array\n";
        cout << "6. Delete Student from Linked List\n";
        cout << "7. Search Student by ID in Array\n";
        cout << "8. Search Student by ID in Linked List\n";
        cout << "9. Calculate Average Grade\n";
        cout << "10. Sort Students by Grade\n";
        cout << "11. Save Data to File\n";
        cout << "12. Load Data from File\n";
        cout << "13. Quit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            manager.addStudentToArray();
            break;
        case 2:
            manager.addStudentToLinkedList();
            break;
        case 3:
            manager.displayStudentsArray();
            break;
        case 4:
            manager.displayStudentsLinkedList();
            break;
        case 5:
            manager.deleteStudentFromArray();
            break;
        case 6:
            manager.deleteStudentFromLinkedList();
            break;
        case 7:
            manager.searchStudentInArray();
            break;
        case 8:
            manager.searchStudentInLinkedList();
            break;
        case 9:
            manager.calculateAverageGrade();
            break;
        case 10:
            manager.sortStudentsByGrade();
            break;
        case 11:
            manager.saveDataToFile();
            break;
        case 12:
            manager.loadDataFromFile();
            break;
        case 13:
            manager.quitSystem();
            break;
        default:
            cout << "Invalid choice, please try again.\n";
        }
    } while (choice != 13);

    return 0;
}
