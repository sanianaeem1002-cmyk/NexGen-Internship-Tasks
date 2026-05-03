#include <iostream>
#include <cmath>
#include <string>
#include <limits>
#include <cstdio>

using namespace std;

void printDivider() {
    cout << "-----------------------------------------\n";
}

void flushInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool isValidNumber(const string &s) {
    if (s.empty()) return false;
    int i = 0;
    int len = s.length();
    if (s[i] == '-' || s[i] == '+') i++;
    if (i == len) return false;
    bool hasDot = false;
    bool hasDigit = false;
    for (; i < len; i++) {
        if (s[i] == '.') {
            if (hasDot) return false;
            hasDot = true;
        } else if (s[i] >= '0' && s[i] <= '9') {
            hasDigit = true;
        } else {
            return false;
        }
    }
    return hasDigit;
}

double toDouble(const string &s) {
    double val = 0.0;
    sscanf(s.c_str(), "%lf", &val);
    return val;
}

bool isWholeNumber(double val) {
    return (val == floor(val));
}

bool askTryAgain() {
    string ans;
    cout << "\nTry again? (y = yes / n = back to menu): ";
    cin >> ans;
    flushInput();
    return (ans == "y" || ans == "Y");
}

// -- CALCULATOR ------------------------------
void runCalculator() {
    cout << "\n=========================================\n";
    cout << "             CALCULATOR\n";
    cout << "=========================================\n\n";
    cout << "Operators : +   -   *   /   %\n";
    cout << "Supports  : negatives, decimals, fractions\n\n";

    while (true) {
        string raw1, raw2, opStr;
        double num1, num2;
        char op;

        // First number
        cout << "Enter first number: ";
        cin >> raw1;
        flushInput();
        if (!isValidNumber(raw1)) {
            cout << "Error: Invalid input.\n";
            if (!askTryAgain()) break;
            continue;
        }
        num1 = toDouble(raw1);

        // Operator
        cout << "Enter operator (+ - * / %): ";
        cin >> opStr;
        flushInput();
        if (opStr.length() != 1 ||
            (opStr[0] != '+' && opStr[0] != '-' &&
             opStr[0] != '*' && opStr[0] != '/' && opStr[0] != '%')) {
            cout << "Error: Invalid operator. Use + - * / %\n";
            if (!askTryAgain()) break;
            continue;
        }
        op = opStr[0];

        // Second number
        cout << "Enter second number: ";
        cin >> raw2;
        flushInput();
        if (!isValidNumber(raw2)) {
            cout << "Error: Invalid input.\n";
            if (!askTryAgain()) break;
            continue;
        }
        num2 = toDouble(raw2);

        // Calculate
        cout << "\n";
        printDivider();
        if      (op == '+') cout << num1 << " + " << num2 << " = " << (num1 + num2) << "\n";
        else if (op == '-') cout << num1 << " - " << num2 << " = " << (num1 - num2) << "\n";
        else if (op == '*') cout << num1 << " x " << num2 << " = " << (num1 * num2) << "\n";
        else if (op == '/') {
            if (num2 == 0) cout << "Error: Cannot divide by zero.\n";
            else cout << num1 << " / " << num2 << " = " << (num1 / num2) << "\n";
        }
        else if (op == '%') {
            if (!isWholeNumber(num1) || !isWholeNumber(num2))
                cout << "Error: Modulo requires whole numbers only.\n";
            else if ((long long)num2 == 0)
                cout << "Error: Cannot modulo by zero.\n";
            else {
                long long a = (long long)num1, b = (long long)num2;
                cout << a << " % " << b << " = " << (a % b) << "\n";
            }
        }
        printDivider();

        if (!askTryAgain()) break;
        cout << "\n";
    }
}

bool isPrime(long long n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (long long i = 3; i <= (long long)sqrt((double)n); i += 2)
        if (n % i == 0) return false;
    return true;
}

bool isPerfect(long long n) {
    if (n <= 1) return false;
    long long sum = 1;
    for (long long i = 2; i <= (long long)sqrt((double)n); i++) {
        if (n % i == 0) { sum += i; if (i != n / i) sum += n / i; }
    }
    return sum == n;
}

bool isArmstrong(long long n) {
    if (n < 0) return false;
    long long temp = n, digits = 0;
    if (n == 0) digits = 1;
    while (temp > 0) { digits++; temp /= 10; }
    temp = n;
    long long sum = 0;
    while (temp > 0) {
        int d = temp % 10;
        long long pw = 1;
        for (int i = 0; i < digits; i++) pw *= d;
        sum += pw; temp /= 10;
    }
    return sum == n;
}

bool isPalindrome(long long n) {
    if (n < 0) return false;
    long long orig = n, rev = 0;
    while (n > 0) { rev = rev * 10 + n % 10; n /= 10; }
    return orig == rev;
}

bool isPerfSq(long long x) {
    if (x < 0) return false;
    long long s = (long long)sqrt((double)x);
    return s * s == x;
}

bool isFibonacci(long long n) {
    if (n < 0) return false;
    return isPerfSq(5*n*n+4) || isPerfSq(5*n*n-4);
}

void runNumberChecker() {
    cout << "\n=========================================\n";
    cout << "           NUMBER CHECKER\n";
    cout << "=========================================\n\n";
    cout << "Checks: Even/Odd | Prime | Perfect\n";
    cout << "        Armstrong | Palindrome | Fibonacci\n";
    cout << "        Positive | Negative | Zero\n\n";

    while (true) {
        string raw;
        double val;

        cout << "Enter a number: ";
        cin >> raw;
        flushInput();

        if (!isValidNumber(raw)) {
            cout << "Error: Invalid input. Please enter a valid number.\n";
            if (!askTryAgain()) break;
            continue;
        }

        val = toDouble(raw);

        cout << "\n";
        printDivider();
        cout << "Results for: " << val << "\n";
        printDivider();

        bool anySpecial = false;

        if      (val > 0) cout << "[+] Positive number\n";
        else if (val < 0) cout << "[-] Negative number\n";
        else              cout << "[=] Zero\n";

        if (isWholeNumber(val)) {
            long long n = (long long)val;
            if (n % 2 == 0) cout << "[+] Even number\n";
            else            cout << "[+] Odd number\n";

            if (isPrime(n))                { cout << "[+] Prime number\n";      anySpecial = true; }
            if (isPerfect(n))              { cout << "[+] Perfect number\n";    anySpecial = true; }
            if (n >= 0 && isArmstrong(n))  { cout << "[+] Armstrong number\n";  anySpecial = true; }
            if (n >= 0 && isPalindrome(n)) { cout << "[+] Palindrome number\n"; anySpecial = true; }
            if (n >= 0 && isFibonacci(n))  { cout << "[+] Fibonacci number\n";  anySpecial = true; }

            if (!anySpecial && n > 1)
                cout << "[i] No special category matched for this number.\n";
        } else {
            cout << "[i] Decimal number -- integer checks skipped.\n";
        }

        printDivider();

        if (!askTryAgain()) break;
        cout << "\n";
    }
}

// -- GRADING SYSTEM ---------------------------
void runGradingSystem() {
    cout << "\n=========================================\n";
    cout << "           GRADING SYSTEM\n";
    cout << "=========================================\n\n";
    cout << "  A+: 95-100  |  A : 90-94\n";
    cout << "  B+: 85-89   |  B : 80-84\n";
    cout << "  C+: 75-79   |  C : 70-74\n";
    cout << "  D+: 65-69   |  D : 60-64\n";
    cout << "  F : 0-59\n\n";

    while (true) {
        string raw;
        double marks;

        cout << "Enter marks (0-100): ";
        cin >> raw;
        flushInput();

        if (!isValidNumber(raw)) {
            cout << "Error: Invalid input. Please enter a number.\n";
            if (!askTryAgain()) break;
            continue;
        }

        marks = toDouble(raw);

        if (marks < 0 || marks > 100) {
            cout << "Error: Marks must be between 0 and 100.\n";
            if (!askTryAgain()) break;
            continue;
        }

        string grade, remarks;
        if      (marks >= 95) { grade = "A+"; remarks = "Outstanding! Perfect performance."; }
        else if (marks >= 90) { grade = "A";  remarks = "Excellent! Keep it up."; }
        else if (marks >= 85) { grade = "B+"; remarks = "Very Good! Almost there."; }
        else if (marks >= 80) { grade = "B";  remarks = "Good work! Room to improve."; }
        else if (marks >= 75) { grade = "C+"; remarks = "Above average. Push harder."; }
        else if (marks >= 70) { grade = "C";  remarks = "Average. Try to do better."; }
        else if (marks >= 65) { grade = "D+"; remarks = "Below average. Need more effort."; }
        else if (marks >= 60) { grade = "D";  remarks = "Poor. Significant improvement needed."; }
        else                  { grade = "F";  remarks = "Fail. Please retake the course."; }

        cout << "\n";
        printDivider();
        cout << "  Marks   : " << marks << " / 100\n";
        cout << "  Grade   : " << grade << "\n";
        cout << "  Remarks : " << remarks << "\n";
        printDivider();

        if (!askTryAgain()) break;
        cout << "\n";
    }
}

// -- MAIN -------------------------------------
int main() {
    cout << "=========================================\n";
    cout << "   NexGen Skills Hub -- C++ Week 1\n";
    cout << "   Fundamentals & Logic Building\n";
    cout << "=========================================\n\n";

    string choice;

    while (true) {
        cout << "\nMAIN MENU\n";
        printDivider();
        cout << "  1. Calculator\n";
        cout << "  2. Number Checker\n";
        cout << "  3. Grading System\n";
        cout << "  0. Exit\n";
        printDivider();
        cout << "Choose (0-3): ";
        cin >> choice;
        flushInput();

        if      (choice == "1") runCalculator();
        else if (choice == "2") runNumberChecker();
        else if (choice == "3") runGradingSystem();
        else if (choice == "0") { cout << "\nGoodbye! Good luck with your internship.\n"; break; }
        else cout << "Invalid choice. Please enter 0, 1, 2, or 3.\n";
    }

    return 0;
}
