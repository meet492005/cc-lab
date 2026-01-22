#include <bits/stdc++.h>
using namespace std;

// ---------- KEYWORDS ----------
set<string> keywords = {
    "auto","break","case","char","const","continue","default","do","double",
    "else","enum","extern","float","for","goto","if","int","long","register",
    "return","short","signed","sizeof","static","struct","switch","typedef",
    "union","unsigned","void","volatile","while","bool"
};

// ---------- OPERATORS ----------
set<string> operators = {
    "+","-","*","/","%","=","==","!=","<",">","<=",">=",
    "&&","||","!","++","--","+=","-=","*=","/=","%=",
    "&","|","^","~","<<",">>","->","."
};

// ---------- PUNCTUATIONS ----------
set<char> punctuations = {
    ';',',','(',')','{','}','[',']'
};

vector<pair<string,string>> tokens; // (type,value)
set<string> symbolTable;
vector<pair<int,string>> lexicalErrors;

// ---------- REMOVE COMMENTS ----------
string removeComments(const string &code) {
    string result;
    bool inSingleLine = false, inMultiLine = false;
    bool inString = false, inChar = false;

    for (int i = 0; i < (int)code.size(); i++) {
        char c = code[i];

        // Toggle string mode
        if (!inChar && c == '"' && !inSingleLine && !inMultiLine) {
            inString = !inString;
            result += c;
            continue;
        }

        // Toggle char mode
        if (!inString && c == '\'' && !inSingleLine && !inMultiLine) {
            inChar = !inChar;
            result += c;
            continue;
        }

        if (!inString && !inChar) {
            // Start single-line comment //
            if (!inMultiLine && i + 1 < (int)code.size() && code[i] == '/' && code[i + 1] == '/') {
                inSingleLine = true;
                i++;
                continue;
            }

            // End single-line comment
            if (inSingleLine && c == '\n') {
                inSingleLine = false;
                result += c;
                continue;
            }

            // Start multi-line comment /* */
            if (!inSingleLine && i + 1 < (int)code.size() && code[i] == '/' && code[i + 1] == '*') {
                inMultiLine = true;
                i++;
                continue;
            }

            // End multi-line comment
            if (inMultiLine && i + 1 < (int)code.size() && code[i] == '*' && code[i + 1] == '/') {
                inMultiLine = false;
                i++;
                continue;
            }

            // Ignore characters inside comments
            if (inSingleLine || inMultiLine) continue;
        }

        result += c;
    }

    return result;
}

// ---------- CHECK IDENTIFIER ----------
bool isValidIdentifier(const string &s) {
    if (s.empty()) return false;
    if (!(isalpha(s[0]) || s[0] == '_')) return false;
    for (char c : s) {
        if (!(isalnum(c) || c == '_')) return false;
    }
    return true;
}

// ---------- CHECK NUMBER ----------
bool isNumber(const string &s) {
    if (s.empty()) return false;
    bool dot = false;

    for (char c : s) {
        if (c == '.') {
            if (dot) return false;
            dot = true;
        } else if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

// ---------- TOKENIZE ----------
void lexicalAnalyze(const string &code) {
    int line = 1;
    int n = code.size();

    for (int i = 0; i < n; i++) {
        char c = code[i];

        // Count line numbers
        if (c == '\n') {
            line++;
            continue;
        }

        // Skip whitespace
        if (isspace(c)) continue;

        // Punctuation
        if (punctuations.count(c)) {
            tokens.push_back({"Punctuation", string(1, c)});
            continue;
        }

        // String constant
        if (c == '"') {
            string str = "\"";
            int startLine = line;
            i++;

            while (i < n && code[i] != '"') {
                if (code[i] == '\n') line++;
                str += code[i++];
            }

            if (i >= n) {
                lexicalErrors.push_back({startLine, "Unterminated string literal"});
            } else {
                str += "\"";
                tokens.push_back({"Constant", str});
            }
            continue;
        }

        // Char constant
        if (c == '\'') {
            string ch = "'";
            int startLine = line;
            i++;

            while (i < n && code[i] != '\'') {
                if (code[i] == '\n') line++;
                ch += code[i++];
            }

            if (i >= n) {
                lexicalErrors.push_back({startLine, "Unterminated char literal"});
            } else {
                ch += "'";
                tokens.push_back({"Constant", ch});
            }
            continue;
        }

        // Operators (check 2-char first)
        string op2 = "";
        if (i + 1 < n) op2 = string() + code[i] + code[i + 1];

        if (operators.count(op2)) {
            tokens.push_back({"Operator", op2});
            i++;
            continue;
        }

        // 1-char operator
        string op1 = string(1, c);
        if (operators.count(op1)) {
            tokens.push_back({"Operator", op1});
            continue;
        }

        // Identifier / Keyword / Constant / Error
        if (isalnum(c) || c == '_' || c == '.') {
            string word;
            int startLine = line;

            while (i < n && (isalnum(code[i]) || code[i] == '_' || code[i] == '.')) {
                word += code[i];
                i++;
            }
            i--;

            // Keyword
            if (keywords.count(word)) {
                tokens.push_back({"Keyword", word});
            }
            // Constant number
            else if (isNumber(word)) {
                tokens.push_back({"Constant", word});
            }
            // Identifier
            else if (isValidIdentifier(word)) {
                tokens.push_back({"Identifier", word});
                symbolTable.insert(word);
            }
            // Lexical error like 7H
            else {
                lexicalErrors.push_back({startLine, word});
            }
            continue;
        }

        // Unknown character error
        lexicalErrors.push_back({line, string(1, c)});
    }
}

// ---------- MAIN ----------
int main() {
    string filename;
    cout << "Enter C source file name: ";
    cin >> filename;

    ifstream file(filename);
    if (!file) {
        cout << "Error: Cannot open file!\n";
        return 0;
    }

    string code((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // Step 1: Remove comments
    string cleanCode = removeComments(code);

    // Step 2: Lexical Analysis
    lexicalAnalyze(cleanCode);

    // Print Tokens
    cout << "\nTOKENS\n";
    for (auto &t : tokens) {
        cout << t.first << ": " << t.second << "\n";
    }

    // Print Lexical Errors
    cout << "\nLEXICAL ERRORS\n";
    if (lexicalErrors.empty()) {
        cout << "No lexical errors found.\n";
    } else {
        for (auto &e : lexicalErrors) {
            cout << "Line " << e.first << " : " << e.second << " invalid lexeme\n";
        }
    }

    // Print Symbol Table
    cout << "\nSYMBOL TABLE ENTRIES\n";
    int idx = 1;
    for (auto &id : symbolTable) {
        cout << idx++ << ") " << id << "\n";
    }

    return 0;
}
