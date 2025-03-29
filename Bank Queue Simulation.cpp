#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <termios.h>
#include <unistd.h>
using namespace std;

struct Account {
    string firstName, lastName, gender, phoneNumber, password;
    double balance = 0, loanAmount = 0;
    vector<string> transactions;
};

unordered_map<string, Account> accounts;
string loggedInUser = "";
queue<string> bankQueue;

// Save accounts to file
void saveAccounts() {
    ofstream file("accounts.dat", ios::binary);
    for (auto &pair : accounts) {
        file.write((char*)&pair.second, sizeof(Account));
    }
    file.close();
}

// Load accounts from file
void loadAccounts() {
    ifstream file("accounts.dat", ios::binary);
    Account acc;
    while (file.read((char*)&acc, sizeof(Account))) {
        accounts[acc.phoneNumber] = acc;
    }
    file.close();
}

// Function to hide password input
string getPassword() {
    string password;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    cin >> password;
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    cout << endl;
    return password;
}

void createAccount() {
    Account acc;
    cout << "Enter First Name: "; cin >> acc.firstName;
    cout << "Enter Last Name: "; cin >> acc.lastName;
    cout << "Enter Gender (Male/Female): "; cin >> acc.gender;
    cout << "Enter Phone Number: "; cin >> acc.phoneNumber;
    if (accounts.find(acc.phoneNumber) != accounts.end()) {
        cout << "Account already exists!\n";
        return;
    }
    
    cout << "Create Password: ";
    acc.password = getPassword();
    
    accounts[acc.phoneNumber] = acc;
    saveAccounts();
    cout << "Account created successfully!\n";
}

bool login() {
    string phone, pass;
    cout << "Enter Phone Number: "; cin >> phone;
    cout << "Enter Password: ";
    pass = getPassword();
    
    if (accounts.find(phone) != accounts.end() && accounts[phone].password == pass) {
        loggedInUser = phone;
        cout << "Login successful!\n";
        return true;
    }
    cout << "Invalid credentials!\n";
    return false;
}

void deposit() {
    if (loggedInUser.empty()) return;
    double amount;
    cout << "Enter deposit amount: "; cin >> amount;
    if (amount <= 0) { cout << "Invalid amount!\n"; return; }
    accounts[loggedInUser].balance += amount;
    accounts[loggedInUser].transactions.push_back("Deposited: " + to_string(amount));
    saveAccounts();
    cout << "Deposit successful!\n";
}

void withdraw() {
    if (loggedInUser.empty()) return;
    double amount;
    cout << "Enter withdrawal amount: "; cin >> amount;
    if (amount <= 0 || accounts[loggedInUser].balance < amount) {
        cout << "Insufficient funds or invalid amount!\n";
        return;
    }
    accounts[loggedInUser].balance -= amount;
    accounts[loggedInUser].transactions.push_back("Withdrawn: " + to_string(amount));
    saveAccounts();
    cout << "Withdrawal successful!\n";
}

void requestLoan() {
    if (loggedInUser.empty()) return;
    double amount;
    cout << "Enter loan amount: "; cin >> amount;
    if (amount <= 0) { cout << "Invalid amount!\n"; return; }
    accounts[loggedInUser].loanAmount += amount;
    accounts[loggedInUser].transactions.push_back("Loan Taken: " + to_string(amount));
    saveAccounts();
    cout << "Loan granted!\n";
}

void payLoan() {
    if (loggedInUser.empty()) return;
    double amount;
    cout << "Enter loan repayment amount: "; cin >> amount;
    if (amount <= 0 || accounts[loggedInUser].loanAmount < amount) {
        cout << "Invalid amount!\n";
        return;
    }
    accounts[loggedInUser].loanAmount -= amount;
    accounts[loggedInUser].transactions.push_back("Loan Paid: " + to_string(amount));
    saveAccounts();
    cout << "Loan payment successful!\n";
}

void accountStatement() {
    if (loggedInUser.empty()) return;
    cout << "\nAccount Statement for " << accounts[loggedInUser].firstName << " " << accounts[loggedInUser].lastName << "\n";
    cout << "Balance: " << accounts[loggedInUser].balance << "\n";
    cout << "Loan Due: " << accounts[loggedInUser].loanAmount << "\nTransactions:\n";
    for (const string &t : accounts[loggedInUser].transactions) {
        cout << "- " << t << "\n";
    }
}

void mainMenu() {
    int choice;
    while (true) {
        cout << "\n1. Deposit\n2. Withdraw\n3. Request Loan\n4. Pay Loan\n5. Account Statement\n6. Logout\nEnter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: deposit(); break;
            case 2: withdraw(); break;
            case 3: requestLoan(); break;
            case 4: payLoan(); break;
            case 5: accountStatement(); break;
            case 6: loggedInUser = ""; cout << "Logged out.\n"; return;
            default: cout << "Invalid choice!\n";
        }
    }
}

void simulateClientEntryExit() {
    if (accounts.empty()) {
        cout << "No clients in the system.\n";
        return;
    }

    // Iterate over all accounts and simulate entry/exit for each client
    for (auto &pair : accounts) {
        Account &acc = pair.second;
        time_t now = time(0);
        char* entryTime = ctime(&now);
        cout << acc.firstName << " " << acc.lastName << " entered at: " << entryTime;

        // Simulate the client's time spent inside
        sleep(3);  // Simulate time spent by the client (change to suit your needs)

        now = time(0);
        char* exitTime = ctime(&now);
        cout << acc.firstName << " " << acc.lastName << " exited at: " << exitTime;
    }
}

int main() {
    loadAccounts();
    int choice;
    while (true) {
        cout << "\n1. Create Account\n2. Login\n3. Simulation (Entry/Exit Time)\n4. Exit\nEnter choice: ";
        cin >> choice;
        if (choice == 1) createAccount();
        else if (choice == 2 && login()) mainMenu();
        else if (choice == 3) simulateClientEntryExit();
        else if (choice == 4) break;
    }
    return 0;
}
