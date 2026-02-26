#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <iomanip>
#include <stack>

using namespace std;


typedef map<char, map<char, vector<string>>> ParsingTable;

void buildTable(ParsingTable &table) {
    table['S']['a'] = {"ABC", "D"};
    table['S']['b'] = {"ABC"};
    table['S']['c'] = {"ABC", "D"};
    table['S']['('] = {"ABC", "D"};

   
    table['A']['a'] = {"a"};
    table['A']['b'] = {"#"}; 
    table['A']['c'] = {"#"};
    table['A']['('] = {"#"};

    
    table['B']['b'] = {"b"};
    table['B']['c'] = {"#"};
    table['B']['('] = {"#"};


    table['C']['('] = {"(S)"};
    table['C']['c'] = {"c"};

    
    table['D']['a'] = {"AC"};
    table['D']['c'] = {"AC"};
    table['D']['('] = {"AC"};
}

void printTable(ParsingTable &table, vector<char> terminals) {
    cout << "\n--- Predictive Parsing Table ---" << endl;
    
    cout << setw(8) << " ";
    for (char t : terminals) {
        cout << setw(12) << t;
    }
    cout << "\n" << string(85, '-') << endl;
    char nonTerminals[] = {'A', 'B', 'C', 'D', 'S'};

    for (char nt : nonTerminals) {
        cout << setw(5) << nt << " |";
        for (char t : terminals) {
            string cellContent = "";
            if (table[nt].count(t)) {
                for (size_t i = 0; i < table[nt][t].size(); ++i) {
                    string p = table[nt][t][i];
                    cellContent += (p == "#" ? " " : p) + (i < table[nt][t].size() - 1 ? " " : "");
                }
            }
            cout << setw(12) << (cellContent.empty() ? "" : cellContent);
        }
        cout << endl;
    }
}

bool isLL1(ParsingTable &table) {
    bool ll1 = true;
    for (auto const& outer : table) {
        for (auto const& inner : outer.second) {
            if (inner.second.size() > 1) {
                cout << "Multiple entries found at [" << outer.first << ", " << inner.first << "]" << endl;
                ll1 = false;
            }
        }
    }
    return ll1;
}

bool validateString(string input, ParsingTable &table) {
    stack<char> s;
    s.push('$');
    s.push('S'); 
    input += '$';
    int i = 0;

    while (!s.empty()) {
        char top = s.top();
        s.pop();

        if (top == input[i]) {
            i++;
        } else if (top >= 'A' && top <= 'Z') {
            if (table[top].count(input[i])) {
                
                string prod = table[top][input[i]][0];
                if (prod != "#") { // If not epsilon
                    for (int j = prod.length() - 1; j >= 0; j--) {
                        s.push(prod[j]);
                    }
                }
            } else return false;
        } else return false;
    }
    return i == input.length();
}

int main() {
    ParsingTable table;
    vector<char> terminals = {'$', '(', ')', 'a', 'b', 'c'};
    
    buildTable(table);
    printTable(table, terminals);

    cout << "\nLL(1) Validation:" << endl;
    if (isLL1(table)) {
        cout << "The grammar is LL(1)." << endl;
    } else {
        cout << "The grammar is NOT LL(1)." << endl;
    }

    
    vector<string> tests = {"abc", "ac", "(abc)", "c", "(ac)", "a", "()", "(ab)", "abcabc", "b"};
    
    cout << "\nInput Validation Results:" << endl;
    for (string t : tests) {
        cout << "String: " << setw(8) << t << " -> " 
             << (validateString(t, table) ? "Valid string" : "Invalid string") << endl;
    }

    return 0;
}