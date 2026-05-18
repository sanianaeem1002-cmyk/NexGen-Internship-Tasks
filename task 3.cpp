#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <limits>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

// ============================================================
//  GLOBAL CONSTANTS & STRUCTURES
// ============================================================
const string RESULTS_FILE = "QuizResults.txt";

struct Question {
    string text;
    string options[4];
    int correct;

    Question(string q, string o1, string o2, string o3, string o4, int c) {
        text = q;
        options[0] = o1; options[1] = o2; options[2] = o3; options[3] = o4;
        correct = c;
    }
};

struct Subject {
    string name;
    vector<Question> questions;
    bool isSubmitted;
    int score;
    vector<int> userAnswers;

    Subject() : isSubmitted(false), score(0) {}
};

// ============================================================
//  UTILITY FUNCTIONS
// ============================================================
void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void flushBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string getCurrentDateTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[64];
    if (ltm) {
        strftime(buf, sizeof(buf), "%d-%m-%Y  %H:%M:%S", ltm);
        return string(buf);
    }
    return "00-00-0000 00:00:00";
}

// ============================================================
//  QUIZ OPERATIONS MODULE
// ============================================================
class QuizManager {
private:
    string sName, sRoll, sClass, sSec;
    Subject subs[4];
    int timeLimit;

    void saveToFile(int totalObtained) {
        ofstream outFile(RESULTS_FILE.c_str(), ios::app);

        if (outFile.is_open()) {
            outFile << "====================================\n";
            outFile << "         STUDENT QUIZ RESULT        \n";
            outFile << "====================================\n";
            outFile << "Name     : " << sName      << "\n";
            outFile << "Roll No  : " << sRoll      << "\n";
            outFile << "Class    : " << sClass     << "\n";
            outFile << "Section  : " << sSec       << "\n";
            outFile << "Date/Time: " << getCurrentDateTime() << "\n";
            outFile << "------------------------------------\n";
            outFile << left << setw(26) << "SUBJECT" << setw(8) << "MARKS" << "STATUS\n";
            outFile << "------------------------------------\n";

            for (int i = 0; i < 4; i++) {
                string status = !subs[i].isSubmitted ? "NOT TAKEN" : (subs[i].score >= 3 ? "PASS" : "FAIL");
                
                // Fixed: Formatting marks using standard stringstream instead of to_string()
                stringstream scoreStr;
                scoreStr << subs[i].score << "/5";

                outFile << left << setw(26) << subs[i].name
                        << setw(8)  << scoreStr.str()
                        << status   << "\n";
            }

            outFile << "------------------------------------\n";
            outFile << "GRAND TOTAL : " << totalObtained << "/20\n";
            outFile << "FINAL STATUS: " << (totalObtained >= 10 ? "PASS" : "FAIL") << "\n";
            outFile << "====================================\n\n";
            outFile.close();
            cout << "\n>>> [SAVED]: Stored in '" << RESULTS_FILE << "'\n";
        } else {
            cout << "\n>>> [ERROR]: Could not open results file.\n";
        }
    }

    void viewPastResults() {
        clearScreen();
        cout << "====================================\n";
        cout << "        ALL PAST QUIZ RESULTS       \n";
        cout << "====================================\n\n";

        ifstream inFile(RESULTS_FILE.c_str());
        if (!inFile.is_open()) {
            cout << ">>> [INFO]: No results found.\n";
        } else {
            string line;
            bool hasContent = false;
            while (getline(inFile, line)) {
                cout << line << "\n";
                hasContent = true;
            }
            inFile.close();
            if (!hasContent) cout << ">>> [INFO]: File is empty.\n";
        }
        cout << "\nPress Enter to go back...";
        flushBuffer();
    }

    void clearResultsFile() {
        clearScreen();
        cout << "Are you sure you want to clear all data? (Y/N): ";
        char ch;
        cin >> ch;
        flushBuffer();

        if (ch == 'Y' || ch == 'y') {
            ofstream outFile(RESULTS_FILE.c_str(), ios::trunc);
            if (outFile.is_open()) {
                outFile.close();
                cout << ">>> [DONE]: Cleared successfully.\n";
            }
        }
        cout << "\nPress Enter to continue...";
        cin.get();
    }

    void startQuiz(int id) {
        clearScreen();
        cout << "Subject : " << subs[id].name << "\nPress Enter to Start...";
        flushBuffer();

        time_t startTime = time(0);
        bool timeOut = false;

        for (int i = 0; i < 5; i++) {
            int elapsed = (int)(time(0) - startTime);
            int remaining = timeLimit - elapsed;
            if (remaining <= 0) { timeOut = true; break; }

            clearScreen();
            cout << "TIME REMAINING: " << remaining << "s\n------------------------------------\n";
            cout << "Q" << i + 1 << ": " << subs[id].questions[i].text << "\n";
            for (int j = 0; j < 4; j++)
                cout << "  " << j + 1 << ". " << subs[id].questions[i].options[j] << "\n";

            int ans;
            while (true) {
                cout << "Your Choice (1-4): ";
                if (!(cin >> ans) || ans < 1 || ans > 4) {
                    if ((int)(time(0) - startTime) >= timeLimit) { timeOut = true; break; }
                    cout << ">> INVALID!\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                break;
            }
            if (timeOut) break;

            subs[id].userAnswers.push_back(ans);
            if (ans == subs[id].questions[i].correct) subs[id].score++;
        }

        subs[id].isSubmitted = true;
        while ((int)subs[id].userAnswers.size() < 5) subs[id].userAnswers.push_back(0);
        cout << "\nQuiz Finished. Press Enter...";
        flushBuffer();
    }

    void reviewAnswers(int id) {
        clearScreen();
        if (!subs[id].isSubmitted) {
            cout << ">>> Quiz not taken yet!\n";
        } else {
            for (int i = 0; i < 5; i++) {
                int userAns = subs[id].userAnswers[i];
                int correctAns = subs[id].questions[i].correct;
                cout << "Q" << i + 1 << ": " << subs[id].questions[i].text << "\n";
                if (userAns == 0) cout << "  NOT ANSWERED\n";
                else cout << "  Status: " << (userAns == correctAns ? "CORRECT\n" : "WRONG\n");
                cout << "  Correct Answer: " << subs[id].questions[i].options[correctAns - 1] << "\n------------------------------------\n";
            }
        }
        cout << "\nPress Enter to go back...";
        flushBuffer();
    }

    void showFinalReport() {
        int totalObtained = 0;
        for (int i = 0; i < 4; i++) if (subs[i].isSubmitted) totalObtained += subs[i].score;
        saveToFile(totalObtained);

        while (true) {
            clearScreen();
            cout << "====================================================\n";
            cout << "                  FINAL RESULT CARD                 \n";
            cout << "====================================================\n";
            cout << " Name: " << sName << " | Roll No: " << sRoll << "\n----------------------------------------------------\n";
            for (int i = 0; i < 4; i++) {
                stringstream finalScoreStr;
                finalScoreStr << subs[i].score << "/5";

                cout << left << setw(26) << subs[i].name << setw(10) << finalScoreStr.str()
                     << (!subs[i].isSubmitted ? "NOT TAKEN\n" : (subs[i].score >= 3 ? "PASS\n" : "FAIL\n"));
            }
            cout << "----------------------------------------------------\nGRAND TOTAL: " << totalObtained << "/20\n====================================================\n";
            cout << "\n1-4. Review Subjects | 5. Past Results | 6. Clear File | 7. Exit\nChoice: ";

            int revChoice;
            if (!(cin >> revChoice)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
            if (revChoice == 7) break;
            else if (revChoice == 5) viewPastResults();
            else if (revChoice == 6) clearResultsFile();
            else if (revChoice >= 1 && revChoice <= 4) reviewAnswers(revChoice - 1);
        }
    }

public:
    QuizManager() {
        timeLimit = 60;
        subs[0].name = "COAL"; subs[1].name = "OOP"; subs[2].name = "Professional Practices"; subs[3].name = "Entrepreneurship";
        loadBank();
    }

    void loadBank() {
        subs[0].questions.push_back(Question("Which register is known as the counter register?", "AX","BX","CX","DX",3));
        subs[0].questions.push_back(Question("Size of a 'Word' in 8086 architecture is?", "8 bits","16 bits","32 bits","64 bits",2));
        subs[0].questions.push_back(Question("Which flag is set if the result of an operation is zero?","SF","ZF","CF","PF",2));
        subs[0].questions.push_back(Question("What does BIOS stand for?", "Basic Input Output Sys","Binary Input Output Sys","Basic Intel OS","None",1));
        subs[0].questions.push_back(Question("The 'MOV' instruction belongs to which category?", "Arithmetic","Logical","Data Transfer","Control",3));

        subs[1].questions.push_back(Question("Which feature represents 'Data Hiding'?", "Inheritance","Encapsulation","Polymorphism","Abstraction",2));
        subs[1].questions.push_back(Question("A class can have how many destructors?", "One","Two","Many","None",1));
        subs[1].questions.push_back(Question("Which keyword is used for inheritance in C++?", "extends","inherits","using ':' operator","new",3));
        subs[1].questions.push_back(Question("What is a virtual function used for?", "Encapsulation","Runtime Polymorphism","Static Binding","Data Hiding",2));
        subs[1].questions.push_back(Question("Objects are created from?", "Methods","Functions","Classes","Namespaces",3));

        subs[2].questions.push_back(Question("What is 'Integrity'?", "Honesty","Lying","Copying code","Laziness",1));
        subs[2].questions.push_back(Question("IEEE stands for?", "Institute of Engineers","Institute of Electrical and Electronics Engineers","International Energy Ent","None",2));
        subs[2].questions.push_back(Question("Software piracy is?", "Legal","Illegal copying","Buying software","Open source",2));
        subs[2].questions.push_back(Question("A professional should prioritize?", "Money","Public Safety","Vacation","Ego",2));
        subs[2].questions.push_back(Question("Which is a core ethical value?", "Deception","Fairness","Plagiarism","Greed",2));

        subs[3].questions.push_back(Question("An entrepreneur is a person who?", "Works for a boss","Takes risks","Avoids work","Does not innovate",2));
        subs[3].questions.push_back(Question("What is a 'Niche'?", "A big market","A specific segment","A bank loan","A competitor",2));
        subs[3].questions.push_back(Question("What is a 'Business Model'?", "A fashion show","How a company makes money","Office design","Logo",2));
        subs[3].questions.push_back(Question("B2B stands for?", "Back to Business","Business to Business","Bank to Bank","None",2));
        subs[3].questions.push_back(Question("What is 'Equity'?", "Loan","Ownership interest","Debt","Taxes",2));
    }

    void registerStudent() {
        clearScreen();
        cout << "====================================\n        STUDENT REGISTRATION        \n====================================\n";
        cout << "Enter Name    : "; getline(cin >> ws, sName);
        cout << "Enter Roll No : "; getline(cin, sRoll);
        cout << "Enter Class   : "; getline(cin, sClass);
        cout << "Enter Section : "; getline(cin, sSec);
    }

    void run() {
        while (true) {
            clearScreen();
            cout << "====================================\n DASHBOARD  |  Student: " << sName << "\n====================================\n";
            for (int i = 0; i < 4; i++) {
                cout << "  " << i + 1 << ". " << left << setw(26) << subs[i].name << (subs[i].isSubmitted ? "[SUBMITTED]" : "[PENDING]") << "\n";
            }
            cout << "  5. View Past Results\n  6. Exit & Save\n====================================\nSelection: ";
            int choice;
            if (!(cin >> choice)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }

            if (choice == 6) { showFinalReport(); break; }
            else if (choice == 5) viewPastResults();
            else if (choice >= 1 && choice <= 4) {
                int index = choice - 1;
                if (subs[index].isSubmitted) { flushBuffer(); } 
                else startQuiz(index);
            }
        }
    }
};

// ============================================================
//  MAIN EXECUTION
// ============================================================
int main() {
    QuizManager qm;
    qm.registerStudent();
    qm.run();
    return 0;
}
