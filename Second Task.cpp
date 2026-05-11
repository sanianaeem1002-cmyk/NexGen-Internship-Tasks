#include <iostream>
#include <string>
#include <ctime>
#include <limits>
#include <iomanip>
#include <sstream>

using namespace std;

// Configuration constants for the quiz structure
const int MAX_SUBJECTS   = 4;
const int MAX_QUESTIONS  = 5;
const int TIME_LIMIT     = 60;  

// Definition of program data models
struct Question {
    string text;
    string options[4];
    int    correct; 
};

struct Subject {
    string    name;
    Question questions[MAX_QUESTIONS];
    int      userAnswers[MAX_QUESTIONS]; 
    int      score;
    bool      isSubmitted;
};

struct Student {
    string name;
    string roll;
    string className;
    string section;
};

// Global instances for student profile and course data
Student studentInfo;
Subject subjects[MAX_SUBJECTS];

// Functional declarations for program logic
void  loadQuestionBank();
void  registerStudent();
void  startQuiz(int subjectIndex);
void  reviewAnswers(int subjectIndex);
void  showFinalReport();
void  showDashboard();
void  clearScreen();
void  waitForEnter(const string& msg = "Press Enter to continue...");
bool  getIntInput(int& val, int minVal, int maxVal);
void  printDivider(char ch = '-', int width = 50);
void  printHeader(const string& title);

int main() {
    loadQuestionBank(); // Initialize data
    registerStudent();  // Get user profile
    showDashboard();    // Enter main loop
    return 0;
}

// Utility to refresh the console view based on OS
void clearScreen() {
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

// Helper to pause execution for user readability
void waitForEnter(const string& msg) {
    cout << "\n" << msg;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Visual formatting: prints a line of characters
void printDivider(char ch, int width) {
    for (int i = 0; i < width; i++) cout << ch;
    cout << "\n";
}

// Visual formatting: prints a bordered header
void printHeader(const string& title) {
    printDivider('=');
    cout << "  " << title << "\n";
    printDivider('=');
}

// Validates integer input and prevents buffer overflow/skipping
bool getIntInput(int& val, int minVal, int maxVal) {
    if (!(cin >> val)) {
        cin.clear(); // Reset error state
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clean trailing newline
    return (val >= minVal && val <= maxVal);
}

// Hardcoded database for quiz subjects and questions
void loadQuestionBank() {
    // Subject 0 initialization: COAL
    subjects[0].name = "COAL";
    subjects[0].score = 0;
    subjects[0].isSubmitted = false;
    for (int i = 0; i < MAX_QUESTIONS; i++) subjects[0].userAnswers[i] = 0;
    subjects[0].questions[0] = {"Which register is the counter register?", {"AX", "BX", "CX", "DX"}, 3};
    subjects[0].questions[1] = {"8086 Word size?", {"8 bits", "16 bits", "32 bits", "64 bits"}, 2};
    subjects[0].questions[2] = {"Flag set if result is zero?", {"SF", "ZF", "CF", "PF"}, 2};
    subjects[0].questions[3] = {"BIOS stands for?", {"Basic Input Output Sys", "Binary Input Output Sys", "Basic Intel OS", "None"}, 1};
    subjects[0].questions[4] = {"'MOV' category?", {"Arithmetic", "Logical", "Data Transfer", "Control"}, 3};

    // Subject 1 initialization: OOP
    subjects[1].name = "OOP";
    subjects[1].score = 0;
    subjects[1].isSubmitted = false;
    for (int i = 0; i < MAX_QUESTIONS; i++) subjects[1].userAnswers[i] = 0;
    subjects[1].questions[0] = {"Which feature is 'Data Hiding'?", {"Inheritance", "Encapsulation", "Polymorphism", "Abstraction"}, 2};
    subjects[1].questions[1] = {"Max destructors in a class?", {"One", "Two", "Many", "None"}, 1};
    subjects[1].questions[2] = {"Keyword for inheritance?", {"extends", "inherits", "using ':' operator", "new"}, 3};
    subjects[1].questions[3] = {"Virtual function use?", {"Encapsulation", "Runtime Polymorphism", "Static Binding", "Data Hiding"}, 2};
    subjects[1].questions[4] = {"Objects are created from?", {"Methods", "Functions", "Classes", "Namespaces"}, 3};

    // Subject 2 initialization: Professional Practices
    subjects[2].name = "Professional Practices";
    subjects[2].score = 0;
    subjects[2].isSubmitted = false;
    for (int i = 0; i < MAX_QUESTIONS; i++) subjects[2].userAnswers[i] = 0;
    subjects[2].questions[0] = {"What is 'Integrity'?", {"Honesty", "Lying", "Copying code", "Laziness"}, 1};
    subjects[2].questions[1] = {"IEEE stands for?", {"Inst. of Engineers", "Inst. of Electrical and Electronics Engineers", "Int. Energy", "None"}, 2};
    subjects[2].questions[2] = {"Software piracy is?", {"Legal", "Illegal copying", "Buying software", "Open source"}, 2};
    subjects[2].questions[3] = {"A professional prioritizes?", {"Money", "Public Safety", "Vacation", "Ego"}, 2};
    subjects[2].questions[4] = {"Core ethical value?", {"Deception", "Fairness", "Plagiarism", "Greed"}, 2};

    // Subject 3 initialization: Entrepreneurship
    subjects[3].name = "Entrepreneurship";
    subjects[3].score = 0;
    subjects[3].isSubmitted = false;
    for (int i = 0; i < MAX_QUESTIONS; i++) subjects[3].userAnswers[i] = 0;
    subjects[3].questions[0] = {"Entrepreneur is someone who?", {"Works for boss", "Takes risks", "Avoids work", "Does not innovate"}, 2};
    subjects[3].questions[1] = {"What is a 'Niche'?", {"Big market", "Specific segment", "Bank loan", "Competitor"}, 2};
    subjects[3].questions[2] = {"What is a Business Model?", {"Fashion show", "How company makes money", "Office design", "Logo"}, 2};
    subjects[3].questions[3] = {"B2B stands for?", {"Back to Business", "Business to Business", "Bank to Bank", "None"}, 2};
    subjects[3].questions[4] = {"What is 'Equity'?", {"Loan", "Ownership interest", "Debt", "Taxes"}, 2};
}

// Collects student details at program start
void registerStudent() {
    clearScreen();
    printHeader("STUDENT REGISTRATION");
    cout << "Enter Name     : "; getline(cin >> ws, studentInfo.name);
    cout << "Enter Roll No  : "; getline(cin, studentInfo.roll);
    cout << "Enter Class    : "; getline(cin, studentInfo.className);
    cout << "Enter Section  : "; getline(cin, studentInfo.section);
    cout << "\n>> Registration Successful!";
    waitForEnter("Press Enter to go to Dashboard...");
}

// Handles the timed quiz examination for a selected subject
void startQuiz(int id) {
    clearScreen();
    cout << "Subject : " << subjects[id].name << "\n";
    cout << "Time Limit: " << TIME_LIMIT << "s | Questions: " << MAX_QUESTIONS << "\n";
    
    cout << "\nPress Enter to Start...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    time_t startTime = time(0); // Track start time
    bool timeOut = false;

    for (int i = 0; i < MAX_QUESTIONS; i++) {
        int elapsed = (int)(time(0) - startTime);
        int remaining = TIME_LIMIT - elapsed;
        if (remaining <= 0) { timeOut = true; break; }

        clearScreen();
        cout << "  TIME REMAINING : " << remaining << "s\n";
        printDivider();
        cout << "  Q" << i + 1 << ": " << subjects[id].questions[i].text << "\n\n";

        for (int j = 0; j < 4; j++) {
            cout << "  " << j + 1 << ". " << subjects[id].questions[i].options[j] << "\n";
        }

        int ans = 0;
        while (true) {
            cout << "\n  Your Choice (1-4): ";
            if (getIntInput(ans, 1, 4)) {
                if ((int)(time(0) - startTime) >= TIME_LIMIT) { timeOut = true; break; }
                break; // Valid input received
            } else {
                if ((int)(time(0) - startTime) >= TIME_LIMIT) { timeOut = true; break; }
                cout << "  >> INVALID! Enter 1-4.";
            }
        }
        if (timeOut) break;

        subjects[id].userAnswers[i] = ans; // Save user choice
        if (ans == subjects[id].questions[i].correct) subjects[id].score++; // Increment score
    }

    if (timeOut) cout << "\n  >> TIME UP! Quiz ended automatically.\n";
    subjects[id].isSubmitted = true;
    waitForEnter("Returning to menu. Press Enter...");
}

// Displays correct vs incorrect answers for a completed subject
void reviewAnswers(int id) {
    clearScreen();
    if (!subjects[id].isSubmitted) {
        cout << "  >> You haven't taken this quiz yet!\n";
        waitForEnter();
        return;
    }

    printHeader("REVIEW: " + subjects[id].name);
    for (int i = 0; i < MAX_QUESTIONS; i++) {
        int userAns = subjects[id].userAnswers[i];
        int correctAns = subjects[id].questions[i].correct;

        cout << "\n  Q" << i + 1 << ": " << subjects[id].questions[i].text << "\n";
        if (userAns == 0) cout << "  Status         : NOT ANSWERED\n";
        else if (userAns == correctAns) cout << "  Status         : CORRECT\n";
        else cout << "  Status         : WRONG\n  Your Answer   : " << subjects[id].questions[i].options[userAns - 1] << "\n";
        
        cout << "  Correct Ans   : " << subjects[id].questions[i].options[correctAns - 1] << "\n";
        printDivider('-', 40);
    }
    waitForEnter("Press Enter to go back...");
}

// Generates the final performance overview for all subjects
void showFinalReport() {
    while (true) {
        clearScreen();
        printHeader("FINAL RESULT CARD");
        cout << "  Name   : " << studentInfo.name << "  |  Roll : " << studentInfo.roll << "\n";
        cout << "  Class  : " << studentInfo.className << "  |  Sec   : " << studentInfo.section << "\n";
        printDivider();

        cout << left << setw(28) << "  SUBJECT" << setw(10) << "MARKS" << "STATUS\n";
        printDivider();

        int totalObtained = 0;
        for (int i = 0; i < MAX_SUBJECTS; i++) {
            cout << "  " << left << setw(26) << subjects[i].name;
            if (!subjects[i].isSubmitted) {
                cout << setw(10) << "---" << "PENDING\n";
            } else {
                cout << subjects[i].score << "/5      " << (subjects[i].score >= 3 ? "PASS" : "FAIL") << "\n";
                totalObtained += subjects[i].score;
            }
        }
        printDivider();
        cout << "  GRAND TOTAL : " << totalObtained << "/20  (" << (totalObtained >= 10 ? "PASS" : "FAIL") << ")\n";
        printDivider('=');

        cout << "\n  Select 1-4 to Review Subject, 5 to Exit: ";
        int choice;
        if (!getIntInput(choice, 1, 5)) continue;
        if (choice == 5) break;
        reviewAnswers(choice - 1);
    }
}

// Primary navigation menu for the user
void showDashboard() {
    while (true) {
        clearScreen();
        printHeader("DASHBOARD  |  " + studentInfo.name);
        for (int i = 0; i < MAX_SUBJECTS; i++) {
            cout << "  " << i + 1 << ". " << subjects[i].name 
                 << (subjects[i].isSubmitted ? "  [DONE]" : "  [PENDING]") << "\n";
        }
        cout << "\n  5. VIEW FINAL REPORT / EXIT\n\n  Selection: ";
        int choice;
        if (!getIntInput(choice, 1, 5)) continue;
        if (choice == 5) { showFinalReport(); break; }

        if (subjects[choice - 1].isSubmitted) {
            cout << "\n  >> Quiz already completed!";
            waitForEnter();
        } else {
            startQuiz(choice - 1); // Start examination
        }
    }
}

