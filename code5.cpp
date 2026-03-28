#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <cctype>
using namespace std;

// List of C keywords
set<string> keywords = {
    "int","float","char","double","return","void","if","else","while","for",
    "break","continue","struct","long","short","typedef","unsigned","signed"
};

// Operators
set<string> operators = {
    "+","-","*","/","=","==","!=","<",">","<=",">=","&&","||","!","%","++","--"
};

// Punctuation
set<char> punctuation = {'(', ')', '{', '}', '[', ']', ';', ',', '.'};

// Symbol Table
set<string> symbolTable;

// Check functions
bool isKeyword(string s) {
    return keywords.count(s);
}

bool isOperator(string s) {
    return operators.count(s);
}

bool isPunctuation(char c) {
    return punctuation.count(c);
}

bool isValidIdentifier(string s) {
    if (!isalpha(s[0]) && s[0] != '_') return false;
    for (char c : s) {
        if (!isalnum(c) && c != '_') return false;
    }
    return true;
}

bool isNumber(string s) {
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

int main() {
    ifstream file("input.c");  // input file
    if (!file) {
        cout << "File not found!" << endl;
        return 0;
    }

    string line;
    int lineNo = 0;

    cout << "TOKENS\n";
    vector<string> errors;

    while (getline(file, line)) {
        lineNo++;

        // Remove comments
        size_t pos;
        if ((pos = line.find("//")) != string::npos)
            line = line.substr(0, pos);

        string token = "";

        for (int i = 0; i < line.length(); i++) {
            char c = line[i];

            // String handling
            if (c == '"') {
                string str = "\"";
                i++;
                while (i < line.length() && line[i] != '"') {
                    str += line[i];
                    i++;
                }
                str += "\"";
                cout << "String: " << str << endl;
                continue;
            }

            // Character constant
            if (c == '\'') {
                string str = "\'";
                i++;
                while (i < line.length() && line[i] != '\'') {
                    str += line[i];
                    i++;
                }
                str += "\'";
                cout << "Constant: " << str << endl;
                continue;
            }

            // Punctuation
            if (isPunctuation(c)) {
                cout << "Punctuation: " << c << endl;
                continue;
            }

            // Operators
            if (string("+-*/=%<>!&|").find(c) != string::npos) {
                string op(1, c);

                // Check double operators
                if (i + 1 < line.length()) {
                    string two = op + line[i + 1];
                    if (isOperator(two)) {
                        cout << "Operator: " << two << endl;
                        i++;
                        continue;
                    }
                }

                cout << "Operator: " << op << endl;
                continue;
            }

            // Build token
            if (isalnum(c) || c == '_') {
                token += c;
            } else {
                if (!token.empty()) {

                    if (isKeyword(token)) {
                        cout << "Keyword: " << token << endl;
                    }
                    else if (isNumber(token)) {
                        cout << "Constant: " << token << endl;
                    }
                    else if (isValidIdentifier(token)) {
                        cout << "Identifier: " << token << endl;
                        symbolTable.insert(token);
                    }
                    else {
                        errors.push_back("Line " + to_string(lineNo) + " : " + token + " invalid lexeme");
                    }

                    token = "";
                }
            }
        }

        // Last token
        if (!token.empty()) {
            if (isKeyword(token)) {
                cout << "Keyword: " << token << endl;
            }
            else if (isNumber(token)) {
                cout << "Constant: " << token << endl;
            }
            else if (isValidIdentifier(token)) {
                cout << "Identifier: " << token << endl;
                symbolTable.insert(token);
            }
            else {
                errors.push_back("Line " + to_string(lineNo) + " : " + token + " invalid lexeme");
            }
        }
    }

    // Errors
    cout << "\nLEXICAL ERRORS\n";
    for (auto e : errors) {
        cout << e << endl;
    }

    // Symbol Table
    cout << "\nSYMBOL TABLE ENTRIES\n";
    int i = 1;
    for (auto s : symbolTable) {
        cout << i++ << ") " << s << endl;
    }

    return 0;
}