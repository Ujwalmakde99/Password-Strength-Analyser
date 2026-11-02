#include <iostream>
#include <string>
#include <cctype>
#include <random>
#include <algorithm>
using namespace std;

class PasswordAnalyzer {
private:
    string password;
    int length_score;
    bool has_upper, has_lower, has_digit, has_special;
    int strength_score;

public:
    PasswordAnalyzer(const string& pwd) : password(pwd), length_score(0), 
                                          has_upper(false), has_lower(false), 
                                          has_digit(false), has_special(false), 
                                          strength_score(0) {
        analyze();
    }

    void analyze() {
        // Reset flags
        has_upper = has_lower = has_digit = has_special = false;
        
        for (char c : password) {
            if (isupper(c)) has_upper = true;
            else if (islower(c)) has_lower = true;
            else if (isdigit(c)) has_digit = true;
            else has_special = true;
        }

        // Evaluate length
        if (password.length() >= 12) length_score = 2;
        else if (password.length() >= 8) length_score = 1;
        else length_score = 0;

        strength_score = length_score + has_upper + has_lower + has_digit + has_special;
    }

    void displayAnalysis() const {
        cout << "\nPassword Analysis:\n";
        cout << "-------------------\n";
        cout << "Length: " << password.length() << endl;
        cout << "Contains Uppercase: " << (has_upper ? "Yes" : "No") << endl;
        cout << "Contains Lowercase: " << (has_lower ? "Yes" : "No") << endl;
        cout << "Contains Digit: " << (has_digit ? "Yes" : "No") << endl;
        cout << "Contains Special Characters: " << (has_special ? "Yes" : "No") << endl;

        cout << "\nPassword Strength: ";
        if (strength_score <= 2)
            cout << "Weak";
        else if (strength_score == 3 || strength_score == 4)
            cout << "Moderate";
        else
            cout << "Strong";
        cout << endl;
    }

    int getStrengthScore() const { return strength_score; }
};

class PasswordGenerator {
private:
    const string uppercase = "ABCDEFGHJKLMNPQRSTUVWXYZ";  // No I, O
    const string lowercase = "abcdefghijkmnopqrstuvwxyz";  // No l
    const string digits = "23456789";                       // No 0, 1
    const string special = "!@#$%^&*-_=+";
    
    random_device rd;
    mt19937 gen;

public:
    PasswordGenerator() : gen(rd()) {}

    string generate(int length = 12) {
        if (length < 4) {
            cout << "Password too short! Minimum 4 characters needed.\n";
            return "";
        }

        string password = "";
        
        // Guarantee at least one from each category
        password += getRandomChar(uppercase);
        password += getRandomChar(lowercase);
        password += getRandomChar(digits);
        password += getRandomChar(special);

        // Fill remaining with random characters from all pools
        string all_chars = uppercase + lowercase + digits + special;
        for (int i = 4; i < length; i++) {
            password += getRandomChar(all_chars);
        }

        // Shuffle to avoid predictable pattern
        shuffle(password.begin(), password.end(), gen);

        return password;
    }

private:
    char getRandomChar(const string& char_set) {
        uniform_int_distribution<> dis(0, char_set.length() - 1);
        return char_set[dis(gen)];
    }
};

int main() {
    PasswordGenerator generator;
    string password;
    int choice;

    cout << "Password Strength Analyzer & Generator\n";
    cout << "======================================\n\n";
    cout << "1. Analyze existing password\n";
    cout << "2. Generate strong password\n";
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore(); // Clear newline from buffer

    if (choice == 1) {
        cout << "Enter your password: ";
        getline(cin, password);
    } 
    else if (choice == 2) {
        char regenerate;
        do {
            password = generator.generate(12);
            cout << "\nGenerated Password: " << password << endl;
            
            PasswordAnalyzer analyzer(password);
            analyzer.displayAnalysis();
            
            cout << "\nGenerate another? (y/n): ";
            cin >> regenerate;
        } while (tolower(regenerate) == 'y');
        
        return 0;
    }
    else {
        cout << "Invalid choice!\n";
        return 1;
    }

    PasswordAnalyzer analyzer(password);
    analyzer.displayAnalysis();

    return 0;
}