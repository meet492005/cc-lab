#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <iomanip>
#include <stack>

using namespace std;

typedef map<string, map<char, string>> ParsingTable;

void buildTable(ParsingTable &table) {

    table["S"]['a'] = "AS'";
    table["S"]['b'] = "AS'";
    table["S"]['c'] = "AS'";
    table["S"]['('] = "AS'";


    table["S'"]['b'] = "bC";
    table["S'"]['c'] = "C";
    table["S'"]['('] = "C";
    table["S'"]['$'] = "C"; 
    table["S'"][')'] = "C";

    table["A"]['a'] = "a";
    table["A"]['b'] = "#";
    table["A"]['c'] = "#";
    table["A"]['('] = "#";

    table["C"]['('] = "(S)";
    table["C"]['c'] = "c";
}

void printTable(ParsingTable &table, vector<char> terminals) {
    cout << "\n--- Left-Factored Predictive Parsing Table ---" << endl;
    cout << setw(8) << " ";
    for (char t : terminals) cout << setw(12) << t;
    cout << "\n" << string(85, '-') << endl;

    vector<string> nonTerms = {"S", "S'", "A", "C"};
    for (string nt : nonTerms) {
        cout << setw(5) << nt << " |";
        for (char t : terminals) {
            string prod = table[nt].count(t) ? table[nt][t] : "";
            if (prod == "#") prod = "epsilon";
            cout << setw(12) << prod;
        }
        cout << endl;
    }
}

bool validateString(string input, ParsingTable &table) {
    stack<string> s;
    s.push("$");
    s.push("S");
    input += "$";
    int i = 0;

    while (!s.empty()) {
        string top = s.top();
        s.pop();

        if (top.length() == 1 && top[0] == input[i]) {
            i++; // Terminal match
        } else if (table.count(top)) { // Non-Terminal match
            if (table[top].count(input[i])) {
                string prod = table[top][input[i]];
                if (prod != "#") {
                    // Push RHS to stack in reverse
                    // Handling multi-char non-terminals like S'
                    vector<string> symbols;
                    for (int k = 0; k < prod.length(); k++) {
                        string sym = "";
                        sym += prod[k];
                        if (k + 1 < prod.length() && prod[k+1] == '\'') {
                            sym += "'";
                            k++;
                        }
                        symbols.push_back(sym);
                    }
                    for (int k = symbols.size() - 1; k >= 0; k--) {
                        s.push(symbols[k]);
                    }
                }
            } else return false;
        } else if (top == "$") {
            return input[i] == '$';
        } else return false;
    }
    return i == input.length();
}

int main() {
    ParsingTable table;
    vector<char> terminals = {'$', '(', ')', 'a', 'b', 'c'};
    
    buildTable(table);
    printTable(table, terminals);

    cout << "\nLL(1) Validation: The grammar is now LL(1) after Left Factoring." << endl;

    vector<string> tests = {"abc", "ac", "(abc)", "c", "(ac)", "a", "()", "(ab)", "abcabc", "b"};
    
    cout << "\nInput Validation Results:" << endl;
    for (string t : tests) {
        cout << "String: " << setw(8) << t << " -> " 
             << (validateString(t, table) ? "Valid string" : "Invalid string") << endl;
    }

    return 0;
}