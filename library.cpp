#include <iostream>
#include <string>
using namespace std;

const int MAX_BOOKS = 100;
const int MAX_REQUESTS = 100;
const int MAX_RETURNS = 100;

// Enum for book genres
enum Genre {
    FICTION,
    NON_FICTION,
    MYSTERY,
    SCIENCE_FICTION,
    FANTASY,
    BIOGRAPHY,
    HISTORY,
    OTHER
};

class Book {
public:
    string ISBN; // Primary key for the book
    string title;
    string author;
    string publishedDate;
    string publisher;
    Genre genre;
    bool isRequested;

    Book(string isbn = "", string t = "", string a = "", string date = "", string pub = "", Genre g = OTHER) {
        ISBN = isbn;
        title = t;
        author = a;
        publishedDate = date;
        publisher = pub;
        genre = g;
        isRequested = false;
    }

    // Function to convert genre enum to string
    string getGenreString() const {
        switch (genre) {
            case FICTION: return "Fiction";
            case NON_FICTION: return "Non-Fiction";
            case MYSTERY: return "Mystery";
            case SCIENCE_FICTION: return "Science Fiction";
            case FANTASY: return "Fantasy";
            case BIOGRAPHY: return "Biography";
            case HISTORY: return "History";
            default: return "Other";
        }
    }
};

class BookRequestQueue {
private:
    Book requestedBooks[MAX_REQUESTS];
    int front, rear;

public:
    BookRequestQueue() { front = rear = -1; }

    void enqueue(Book book) {
        if (rear >= MAX_REQUESTS - 1) {
            cout << "Queue overflow! Cannot add more book requests.\n";
            return;
        }
        if (front == -1) front = 0;
        requestedBooks[++rear] = book;
    }

    Book dequeue() {
        if (front == -1 || front > rear) {
            cout << "Queue underflow! No book requests to process.\n";
            return Book();
        }
        return requestedBooks[front++];
    }

    bool isEmpty() { return front == -1 || front > rear; }

    Book pop() {
        if (rear < 0) {
            cout << "Stack underflow! No returned books to process.\n";
            return Book();
        }
        return requestedBooks[rear--];
    }

    void display() {
        if (isEmpty()) {
            cout << "No requested books.\n";
            return;
        }
        cout << "Requested Books:\n";
        for (int i = front; i <= rear; i++) {
            cout << "- ISBN: " << requestedBooks[i].ISBN << ", Title: " << requestedBooks[i].title
                 << ", Author: " << requestedBooks[i].author << ", Genre: " << requestedBooks[i].getGenreString() << "\n";
        }
    }
};

class ReturnedBookStack {
private:
    Book returnedBooks[MAX_RETURNS];
    int top;

public:
    ReturnedBookStack() { top = -1; }

    void push(Book book) {
        if (top >= MAX_RETURNS - 1) {
            cout << "Stack overflow! Cannot add more returned books.\n";
            return;
        }
        returnedBooks[++top] = book;
    }

    Book pop() {
        if (top < 0) {
            cout << "Stack underflow! No returned books to process.\n";
            return Book();
        }
        return returnedBooks[top--];
    }

    bool isEmpty() { return top < 0; }

    void display() {
        if (isEmpty()) {
            cout << "No returned books.\n";
            return;
        }
        cout << "Returned Books:\n";
        for (int i = top; i >= 0; i--) {
            cout << "- ISBN: " << returnedBooks[i].ISBN << ", Title: " << returnedBooks[i].title
                 << ", Author: " << returnedBooks[i].author << ", Genre: " << returnedBooks[i].getGenreString() << "\n";
        }
    }
};

class Library {
private:
    Book books[MAX_BOOKS];
    int bookCount;
    BookRequestQueue requestedBooks;
    ReturnedBookStack returnedBooks;

public:
    Library() { bookCount = 0; }

    void addBook(string isbn, string title, string author, string date, string pub, Genre genre) {
        if (bookCount >= MAX_BOOKS) {
            cout << "Library is full! Cannot add more books.\n";
            return;
        }
        books[bookCount++] = Book(isbn, title, author, date, pub, genre);
        cout << "Book added: " << title << " by " << author << " (ISBN: " << isbn << ")\n";
    }

    void requestBook(string isbn) {
        for (int i = 0; i < bookCount; i++) {
            if (books[i].ISBN == isbn && !books[i].isRequested) {
                books[i].isRequested = true;
                requestedBooks.enqueue(books[i]);
                cout << "Book requested: " << books[i].title << " (ISBN: " << isbn << ")\n";
                return;
            }
        }
        cout << "Book not found or already requested.\n";
    }

    void returnBook() {
        if (requestedBooks.isEmpty()) {
            cout << "No books to return.\n";
            return;
        }
        Book book = requestedBooks.pop();

        for (int i = 0; i < bookCount; i++) {
            if (books[i].ISBN == book.ISBN) {
                books[i].isRequested = false;
                break;
            }
        }

        returnedBooks.push(book);
        cout << "Book returned: " << book.title << " (ISBN: " << book.ISBN << ")\n";
    }

    void displayAllBooks() {
        if (bookCount == 0) {
            cout << "No books available in the library.\n";
            return;
        }
        cout << "All Books in the Library:\n";
        for (int i = 0; i < bookCount; i++) {
            cout << "- ISBN: " << books[i].ISBN << ", Title: " << books[i].title
                 << ", Author: " << books[i].author << ", Published: " << books[i].publishedDate
                 << ", Publisher: " << books[i].publisher << ", Genre: " << books[i].getGenreString();
            if (books[i].isRequested) {
                cout << " (Requested)";
            }
            cout << "\n";
        }
    }

    void displayRequestedBooks() {
        requestedBooks.display();
    }

    void displayReturnedBooks() {
        returnedBooks.display();
    }

    void autoRequestBook() {
        if (bookCount == 0) {
            cout << "No books available to request.\n";
            return;
        }
        for (int i = 0; i < bookCount; i++) {
            if (!books[i].isRequested) {
                requestBook(books[i].ISBN);
                return;
            }
        }
        cout << "All books are already requested.\n";
    }

    void autoReturnBook() {
        returnBook();
    }
};

int main() {
    Library library;

    while (true) {
        cout << "\nLibrary Book Management System:\n";
        cout << "1. Add a book\n";
        cout << "2. Request a book\n";
        cout << "3. Return a book\n";
        cout << "4. Display all books\n";
        cout << "5. Display requested books\n";
        cout << "6. Display returned books\n";
        cout << "7. Auto-request a book\n";
        cout << "8. Auto-return a book\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        cin.ignore();
        if (choice == 1) {
            string isbn, title, author, date, pub;
            int genreChoice;
            cout << "Enter book ISBN: ";
            getline(cin, isbn);
            cout << "Enter book title: ";
            getline(cin, title);
            cout << "Enter book author: ";
            getline(cin, author);
            cout << "Enter published date (YYYY-MM-DD): ";
            getline(cin, date);
            cout << "Enter publisher: ";
            getline(cin, pub);
            cout << "Enter genre (0: Fiction, 1: Non-Fiction, 2: Mystery, 3: Science Fiction, 4: Fantasy, 5: Biography, 6: History, 7: Other): ";
            cin >> genreChoice;
            cin.ignore();

            Genre genre = static_cast<Genre>(genreChoice);
            library.addBook(isbn, title, author, date, pub, genre);
        }
        else if (choice == 2) {
            string isbn;
            cout << "Enter book ISBN: ";
            getline(cin, isbn);
            library.requestBook(isbn);
        }
        else if (choice == 3) {
            library.returnBook();
        }
        else if (choice == 4) {
            library.displayAllBooks();
        }
        else if (choice == 5) {
            library.displayRequestedBooks();
        }
        else if (choice == 6) {
            library.displayReturnedBooks();
        }
        else if (choice == 7) {
            library.autoRequestBook();
        }
        else if (choice == 8) {
            library.autoReturnBook();
        }
        else if (choice == 9) {
            break;
        }
        else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
//g++ library.cpp -o library