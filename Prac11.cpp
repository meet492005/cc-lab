#include <iostream>
#include <stack>
#include <vector>
using namespace std;

struct Quad {
    string op, arg1, arg2, result;
};

int priority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

int main() {
    string exp;
    cout << "Enter expression: ";
    getline(cin, exp);   //handles spaces

    stack<string> values;
    stack<char> ops;
    vector<Quad> quads;

    int tempCount = 1;

    for (int i = 0; i < exp.length(); i++) {

        //skip spaces
        if (exp[i] == ' ') continue;

        // If digit - handle multi-digit numbers
        if (isdigit(exp[i])) {
            string num = "";
            while (i < exp.length() && isdigit(exp[i])) {
                num += exp[i++];
            }
            i--;
            values.push(num);
        }

        // If '('
        else if (exp[i] == '(') {
            ops.push(exp[i]);
        }

        // If ')'
        else if (exp[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                string b = values.top(); values.pop();
                string a = values.top(); values.pop();
                char op = ops.top(); ops.pop();

                string temp = "t" + to_string(tempCount++);
                quads.push_back({string(1, op), a, b, temp});
                values.push(temp);
            }
            ops.pop(); // remove '('
        }

        // If operator
        else {
            while (!ops.empty() && priority(ops.top()) >= priority(exp[i])) {
                string b = values.top(); values.pop();
                string a = values.top(); values.pop();
                char op = ops.top(); ops.pop();

                string temp = "t" + to_string(tempCount++);
                quads.push_back({string(1, op), a, b, temp});
                values.push(temp);
            }
            ops.push(exp[i]);
        }
    }

    // Remaining operations
    while (!ops.empty()) {
        string b = values.top(); values.pop();
        string a = values.top(); values.pop();
        char op = ops.top(); ops.pop();

        string temp = "t" + to_string(tempCount++);
        quads.push_back({string(1, op), a, b, temp});
        values.push(temp);
    }

    cout << "\nQuadruple Table:\n";
    cout << "Op\tArg1\tArg2\tResult\n";

    for (auto q : quads) {
        cout << q.op << "\t" << q.arg1 << "\t" << q.arg2 << "\t" << q.result << endl;
    }

    cout<<"\n23CE093 - Dhruv Patel\n";
    return 0;
}
