/*
 * =========================================
 *   NexGen Skills Hub -- C++ Week 1
 *   Fundamentals & Logic Building
 * =========================================
 *
 * This program is a console-based multi-tool menu application with three features:
 *   1. Calculator     -- performs basic arithmetic on two numbers
 *   2. Number Checker -- analyzes a number's mathematical properties
 *   3. Grading System -- converts a marks score into a letter grade
 *
 * The user navigates via a numbered main menu and can loop within each
 * tool or return to the menu at any time.
 */

#include <iostream>
#include <cmath>
#include <string>
#include <limits>
#include <cstdio>

using namespace std;

// Prints a horizontal divider line to visually separate sections in the console
void printDivider() {
    cout << "-----------------------------------------\n";
}

// Clears any error flags on cin and discards leftover characters in the input buffer.
// Called after every cin >> read to prevent stale input from affecting the next read.
void flushInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Validates that a string represents a legal number (integer or decimal).
// Accepts an optional leading '+' or '-' sign.
// Returns false if the string is empty, has multiple dots, or contains non-digit characters.
bool isValidNumber(const string &s) {
    if (s.empty()) return false;
    int i = 0;
    int len = s.length();
    if (s[i] == '-' || s[i] == '+') i++;   // allow one optional sign character
    if (i == len) return false;             // sign-only string is not a valid number
    bool hasDot = false;
    bool hasDigit = false;
    for (; i < len; i++) {
        if (s[i] == '.') {
            if (hasDot) return false;       // two dots ? invalid
            hasDot = true;
        } else if (s[i] >= '0' && s[i] <= '9') {
            hasDigit = true;
        } else {
            return false;                   // any other character ? invalid
        }
    }
    return hasDigit;                        // must contain at least one digit
}

// Converts a validated numeric string to a double using sscanf
double toDouble(const string &s) {
    double val = 0.0;
    sscanf(s.c_str(), "%lf", &val);
    return val;
}

// Returns true if a double value has no fractional part (e.g. 5.0 is whole, 5.3 is not).
// Used to gate integer-only operations like modulo and most number checks.
bool isWholeNumber(double val) {
    return (val == floor(val));
}

// Asks the user whether they want to try the current tool again or go back to the main menu.
// Returns true if the user enters 'y' or 'Y', false otherwise.
bool askTryAgain() {
    string ans;
    cout << "\nTry again? (y = yes / n = back to menu): ";
    cin >> ans;
    flushInput();
    return (ans == "y" || ans == "Y");
}

// ============================================================
//  TOOL 1 -- CALCULATOR
//  Reads two numbers and an operator from the user, then
//  prints the result of the chosen arithmetic operation.
//  Supported operators: +  -  *  /  %
//  Special cases:
//    - Division by zero is caught and reported.
//    - Modulo (%) requires both operands to be whole numbers
//      and the divisor to be non-zero.
// ============================================================
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

        // --- Read and validate the first operand ---
        cout << "Enter first number: ";
        cin >> raw1;
        flushInput();
        if (!isValidNumber(raw1)) {
            cout << "Error: Invalid input.\n";
            if (!askTryAgain()) break;
            continue;
        }
        num1 = toDouble(raw1);

        // --- Read and validate the operator ---
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

        // --- Read and validate the second operand ---
        cout << "Enter second number: ";
        cin >> raw2;
        flushInput();
        if (!isValidNumber(raw2)) {
            cout << "Error: Invalid input.\n";
            if (!askTryAgain()) break;
            continue;
        }
        num2 = toDouble(raw2);

        // --- Perform the selected operation and display the result ---
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
            // Modulo is only defined for integers
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

// ============================================================
//  Helper functions used by the Number Checker tool
// ============================================================

// Returns true if n is a prime number (divisible only by 1 and itself).
// Uses trial division up to sqrt(n) for efficiency.
bool isPrime(long long n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (long long i = 3; i <= (long long)sqrt((double)n); i += 2)
        if (n % i == 0) return false;
    return true;
}

// Returns true if n is a perfect number (sum of its proper divisors equals n).
// Example: 6 = 1+2+3, 28 = 1+2+4+7+14
bool isPerfect(long long n) {
    if (n <= 1) return false;
    long long sum = 1;                          // 1 is always a proper divisor
    for (long long i = 2; i <= (long long)sqrt((double)n); i++) {
        if (n % i == 0) { sum += i; if (i != n / i) sum += n / i; }
    }
    return sum == n;
}

// Returns true if n is an Armstrong (narcissistic) number:
// the sum of each digit raised to the power of the total digit count equals n.
// Example (3-digit): 153 = 1³ + 5³ + 3³
bool isArmstrong(long long n) {
    if (n < 0) return false;
    long long temp = n, digits = 0;
    if (n == 0) digits = 1;
    while (temp > 0) { digits++; temp /= 10; }  // count digits
    temp = n;
    long long sum = 0;
    while (temp > 0) {
        int d = temp % 10;                       // extract last digit
        long long pw = 1;
        for (int i = 0; i < digits; i++) pw *= d; // d ^ digits
        sum += pw; temp /= 10;
    }
    return sum == n;
}

// Returns true if the decimal digits of n read the same forwards and backwards.
// Example: 121, 1331, 12321
bool isPalindrome(long long n) {
    if (n < 0) return false;
    long long orig = n, rev = 0;
    while (n > 0) { rev = rev * 10 + n % 10; n /= 10; }  // build reversed number
    return orig == rev;
}

// Helper: returns true if x is a perfect square (used inside isFibonacci).
bool isPerfSq(long long x) {
    if (x < 0) return false;
    long long s = (long long)sqrt((double)x);
    return s * s == x;
}

// Returns true if n belongs to the Fibonacci sequence.
// Uses the mathematical property: n is Fibonacci iff 5n²+4 or 5n²-4 is a perfect square.
bool isFibonacci(long long n) {
    if (n < 0) return false;
    return isPerfSq(5*n*n+4) || isPerfSq(5*n*n-4);
}

// ============================================================
//  TOOL 2 -- NUMBER CHECKER
//  Reads a single number and reports every mathematical
//  property that applies to it:
//    - Sign        : Positive / Negative / Zero
//    - Parity      : Even / Odd          (integers only)
//    - Prime                              (integers only)
//    - Perfect                            (integers only)
//    - Armstrong                          (non-negative integers only)
//    - Palindrome                         (non-negative integers only)
//    - Fibonacci                          (non-negative integers only)
//  Decimal (non-whole) numbers skip all integer-only checks.
// ============================================================
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

        bool anySpecial = false;    // tracks whether any special category was matched

        // --- Sign check (applies to all numbers including decimals) ---
        if      (val > 0) cout << "[+] Positive number\n";
        else if (val < 0) cout << "[-] Negative number\n";
        else              cout << "[=] Zero\n";

        // --- Integer-only checks ---
        if (isWholeNumber(val)) {
            long long n = (long long)val;

            // Parity
            if (n % 2 == 0) cout << "[+] Even number\n";
            else            cout << "[+] Odd number\n";

            // Special number categories
            if (isPrime(n))                { cout << "[+] Prime number\n";      anySpecial = true; }
            if (isPerfect(n))              { cout << "[+] Perfect number\n";    anySpecial = true; }
            if (n >= 0 && isArmstrong(n))  { cout << "[+] Armstrong number\n";  anySpecial = true; }
            if (n >= 0 && isPalindrome(n)) { cout << "[+] Palindrome number\n"; anySpecial = true; }
            if (n >= 0 && isFibonacci(n))  { cout << "[+] Fibonacci number\n";  anySpecial = true; }

            // If no special property matched (and n > 1 so 0/1 don't get this message)
            if (!anySpecial && n > 1)
                cout << "[i] No special category matched for this number.\n";
        } else {
            // Decimal numbers cannot be tested for the integer-based properties above
            cout << "[i] Decimal number -- integer checks skipped.\n";
        }

        printDivider();

        if (!askTryAgain()) break;
        cout << "\n";
    }
}

// ============================================================
//  TOOL 3 -- GRADING SYSTEM
//  Reads a marks score (0-100) and maps it to a letter grade
//  with a short remark:
//    A+ (95-100), A (90-94), B+ (85-89), B (80-84),
//    C+ (75-79),  C (70-74), D+ (65-69), D (60-64), F (0-59)
// ============================================================
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

        // Validate that the input is a number
        if (!isValidNumber(raw)) {
            cout << "Error: Invalid input. Please enter a number.\n";
            if (!askTryAgain()) break;
            continue;
        }

        marks = toDouble(raw);

        // Validate that the marks are within the acceptable range
        if (marks < 0 || marks > 100) {
            cout << "Error: Marks must be between 0 and 100.\n";
            if (!askTryAgain()) break;
            continue;
        }

        // --- Determine grade and remark based on score thresholds ---
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

        // --- Display the result ---
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

// ============================================================
//  MAIN -- Entry point
//  Displays the main menu in a loop and dispatches to the
//  chosen tool. Entering 0 exits the program.
// ============================================================
int main() {
    cout << "=========================================\n";
    cout << "   NexGen Skills Hub -- C++ Week 1\n";
    cout << "   Fundamentals & Logic Building\n";
    cout << "=========================================\n\n";

    string choice;

    while (true) {
        // Display the main menu options
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

        // Route the user's choice to the appropriate tool
        if      (choice == "1") runCalculator();
        else if (choice == "2") runNumberChecker();
        else if (choice == "3") runGradingSystem();
        else if (choice == "0") { cout << "\nGoodbye! Good luck with your internship.\n"; break; }
        else cout << "Invalid choice. Please enter 0, 1, 2, or 3.\n";
    }

    return 0;
}
