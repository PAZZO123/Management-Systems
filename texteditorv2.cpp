#include <iostream>
#include <ncurses.h>
#include <stack>
#include <string>
#include <fstream>
using namespace std;

class TextEditor
{
private:
    string currentText;
    stack<string> undoStack;
    stack<string> redoStack;

public:
    void init()
    {
        initscr();
        raw();
        keypad(stdscr, TRUE);
        noecho();
        curs_set(1);
        refresh();
        loadFromFile();
    }

    void end()
    {
        endwin();
    }

    void loadFromFile()
    {
        ifstream file("saved_text.txt");
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                currentText += line + "\n";
            }
            file.close();
        }
    }

    void typeText(char c)
    {
        undoStack.push(currentText);
        currentText += c;
        while (!redoStack.empty())
            redoStack.pop();
    }

    void deleteText()
    {
        if (currentText.empty())
            return;
        undoStack.push(currentText);
        currentText.pop_back();
        while (!redoStack.empty())
            redoStack.pop();
    }

    void deleteWord()
    {
        if (currentText.empty())
            return;
        undoStack.push(currentText);
        size_t pos = currentText.find_last_of(" ");
        currentText = (pos == string::npos) ? "" : currentText.substr(0, pos);
        while (!redoStack.empty())
            redoStack.pop();
    }

    void undo()
    {
        if (undoStack.empty())
            return;
        redoStack.push(currentText);
        currentText = undoStack.top();
        undoStack.pop();
    }

    void redo()
    {
        if (redoStack.empty())
            return;
        undoStack.push(currentText);
        currentText = redoStack.top();
        redoStack.pop();
    }

    void pressEnter()
    {
        undoStack.push(currentText);
        currentText += "\n";
        while (!redoStack.empty())
            redoStack.pop();
    }

    void saveToFile()
    {
        ofstream file("saved_text.txt");
        if (file.is_open())
        {
            file << currentText;
            file.close();
        }
    }

    void displayText()
    {
        clear();
        mvprintw(0, 0, "Welcome in Computer Engineering L2 Text Editor");
        mvprintw(1, 0, "---------------------------------------------------------------------------------------------------------------------------");
        mvprintw(2, 0, "Text Editor: Type, Undo (Ctrl+Z), Redo (Ctrl+Y), Save (Ctrl+S), Exit (Ctrl+C), Delete Word (Ctrl+D), Enter for new line");
        mvprintw(3, 0, "----------------------------------------------------------------------------------------------------------------------------");
        mvprintw(5, 0, currentText.c_str());
        refresh();
    }
};

int main()
{
    TextEditor editor;
    editor.init();
    char ch;
    bool running = true;
    while (running)
    {
        editor.displayText();
        ch = getch();
        switch (ch)
        {
        case 3:
            running = false;
            break;
        case 26:
            editor.undo();
            break;
        case 25:
            editor.redo();
            break;
        case 4:
            editor.deleteWord();
            break;
        case 127:
            editor.deleteText();
            break;
        case 10:
            editor.pressEnter();
            break;
        case 19:
            editor.saveToFile();
            break;
        default:
            if (ch >= 32 && ch <= 126)
                editor.typeText(ch);
            break;
        }
    }
    editor.end();
    return 0;
}
