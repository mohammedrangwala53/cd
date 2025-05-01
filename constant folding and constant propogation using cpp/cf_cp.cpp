#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <regex>

using namespace std;

// Structure to represent a quadruple
struct Quad {
    string op;
    string arg1;
    string arg2;
    string result;
};

// Function to check if a string is a constant (integer)
bool isConstant(const string& str) {
    return regex_match(str, regex("^-?\\d+$"));
}

// Function to evaluate a constant expression
string evaluate(const string& op, const string& arg1, const string& arg2) {
    try {
        int a = stoi(arg1);
        int b = arg2.empty() ? 0 : stoi(arg2);
        if (op == "+") return to_string(a + b);
        if (op == "-") return to_string(a - b);
        if (op == "*") return to_string(a * b);
        if (op == "/") return b != 0 ? to_string(a / b) : "0";
    } catch (...) {
        return "";
    }
    return "";
}

// Function to perform constant folding and propagation
void optimizeQuadruples(vector<Quad>& quads) {
    map<string, string> constValues;
    bool changed;

    do {
        changed = false;

        for (auto& q : quads) {
            // Propagate constants
            if (constValues.find(q.arg1) != constValues.end()) {
                q.arg1 = constValues[q.arg1];
                changed = true;
            }
            if (!q.arg2.empty() && constValues.find(q.arg2) != constValues.end()) {
                q.arg2 = constValues[q.arg2];
                changed = true;
            }

            // Constant folding
            if (isConstant(q.arg1) && (q.arg2.empty() || isConstant(q.arg2))) {
                if (q.op == "+" || q.op == "-" || q.op == "*" || q.op == "/") {
                    string folded = evaluate(q.op, q.arg1, q.arg2);
                    if (!folded.empty()) {
                        q.op = "=";
                        q.arg1 = folded;
                        q.arg2 = "";
                        constValues[q.result] = folded;
                        changed = true;
                    }
                } else if (q.op == "=") {
                    constValues[q.result] = q.arg1;
                }
            } else if (q.op == "=" && isConstant(q.arg1)) {
                constValues[q.result] = q.arg1;
                changed = true;
            }
        }

    } while (changed);
}

int main() {
    ifstream inputFile("quads.txt");
    vector<Quad> quads;
    string line;

    // Read quadruples from file
    if (inputFile.is_open()) {
        while (getline(inputFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string op, arg1, arg2, result;
            getline(ss, op, ',');
            getline(ss, arg1, ',');
            getline(ss, arg2, ',');
            getline(ss, result);
            quads.push_back({op, arg1, arg2, result});
        }
        inputFile.close();
    } else {
        cout << "Error: Unable to open quads.txt" << endl;
        return 1;
    }

    // Display input quadruples
    cout << "Input Quadruples:\n";
    for (const auto& q : quads) {
        cout << q.op << "," << q.arg1 << "," << q.arg2 << "," << q.result << endl;
    }

    // Perform optimization
    optimizeQuadruples(quads);

    // Display optimized quadruples
    cout << "\nOptimized Quadruples:\n";
    for (const auto& q : quads) {
        cout << q.op << "," << q.arg1 << "," << q.arg2 << "," << q.result << endl;
    }

    return 0;
}
 