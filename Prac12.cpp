#include <iostream>
#include <stack>
#include <sstream>
#include <vector>
using namespace std;

bool isNumber(string s)
{
    for (char c : s)
    {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Perform operation
string evaluate(string a, string b, char op)
{
    int x = stoi(a);
    int y = stoi(b);
    int res;

    if (op == '+') res = x + y;
    else if (op == '-') res = x - y;
    else if (op == '*') res = x * y;
    else if (op == '/') res = x / y;

    return to_string(res);
}

int priority(char op)
{
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

int main()
{
    string exp;
    cout << "Enter expression: ";
    getline(cin, exp);

    stack<string> values;
    stack<char> ops;

    for (int i = 0; i < exp.length(); i++)
    {

        if (exp[i] == ' ') continue;

        // number or variable
        if (isalnum(exp[i]))
        {
            string token = "";
            while (i < exp.length() && isalnum(exp[i]))
            {
                token += exp[i++];
            }
            i--;
            values.push(token);
        }

        else if (exp[i] == '(')
        {
            ops.push(exp[i]);
        }

        else if (exp[i] == ')')
        {
            while (!ops.empty() && ops.top() != '(')
            {
                string b = values.top();
                values.pop();
                string a = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();

                if (isNumber(a) && isNumber(b))
                {
                    values.push(evaluate(a, b, op)); // fold
                }
                else
                {
                    values.push(a + " " + op + " " + b);
                }
            }
            ops.pop();
        }

        else   // operator
        {
            while (!ops.empty() && priority(ops.top()) >= priority(exp[i]))
            {
                string b = values.top();
                values.pop();
                string a = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();

                if (isNumber(a) && isNumber(b))
                {
                    values.push(evaluate(a, b, op)); // fold
                }
                else
                {
                    values.push(a + " " + op + " " + b);
                }
            }
            ops.push(exp[i]);
        }
    }

    // remaining
    while (!ops.empty())
    {
        string b = values.top();
        values.pop();
        string a = values.top();
        values.pop();
        char op = ops.top();
        ops.pop();

        if (isNumber(a) && isNumber(b))
        {
            values.push(evaluate(a, b, op));
        }
        else
        {
            values.push(a + " " + op + " " + b);
        }
    }

    //upto this gives partial correct output
    //input : x + (3 * 5) – 2
    //output : x + 15 - 2
    //input : (22/7) * r * r
    //So below code completes this

    string res = values.top();

    // tokenize properly
    stringstream ss(res);
    vector<string> tokens;
    string tok;

    while (ss >> tok)
    {
        tokens.push_back(tok);
    }

    vector<string> result;

    for (int i = 0; i < tokens.size(); i++)
    {

        // if operator and next exists
        if ((tokens[i] == "+" || tokens[i] == "-") &&
                i > 0 && i < tokens.size() - 1 &&
                isNumber(result.back()) && isNumber(tokens[i + 1]))
        {

            int val = (tokens[i] == "+")
                      ? stoi(result.back()) + stoi(tokens[i + 1])
                      : stoi(result.back()) - stoi(tokens[i + 1]);

            result.pop_back();
            result.push_back(to_string(val));
            i++; // skip next token
        }
        else
        {
            result.push_back(tokens[i]);
        }
    }

    // print properly
    cout << "\nOptimized Expression: ";
    for (string s : result) cout << s << " ";
    cout << endl;

    cout<<"\n23CE093 - Dhruv Patel\n";
    return 0;
}
