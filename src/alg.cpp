// Copyright 2025 NNTU-CS
#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include "tstack.h"

using std::string;
using std::istringstream;
using std::runtime_error;
using std::isdigit;
using std::isspace;
using std::stoi;

static int precedence(char op) {
    return (op == '+' || op == '-') ? 1 : (op == '*' || op == '/') ? 2 : 0;
}

static inline bool isOp(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

string infx2pstfx(const string& inf) {
    TStack<char, 128> ops;
    string out;
    for (size_t i = 0; i < inf.size(); ++i) {
        char ch = inf[i];
        if (isspace(static_cast<unsigned char>(ch))) {
            continue;
        }
        if (isdigit(static_cast<unsigned char>(ch))) {
            while (i < inf.size() && isdigit(static_cast<unsigned
                char>(inf[i]))) {
                out += inf[i++];
            }
            out += ' ';
            --i;
        } else if (ch == '(') {
            ops.add(ch);
        } else if (ch == ')') {
            while (!ops.isVoid() && ops.getTop() != '(') {
                out += ops.remove();
                out += ' ';
            }
            if (!ops.isVoid()) ops.remove();
        } else if (isOp(ch)) {
            while (!ops.isVoid() && isOp(ops.getTop()) &&
                   precedence(ops.getTop()) >= precedence(ch)) {
                out += ops.remove();
                out += ' ';
            }
            ops.add(ch);
        }
    }
    while (!ops.isVoid()) {
        out += ops.remove();
        out += ' ';
    }
    if (!out.empty() && out.back() == ' ') out.pop_back();
    return out;
}

int eval(const string& post) {
    TStack<int, 128> st;
    istringstream ss(post);
    string token;
    while (ss >> token) {
        if (token.size() == 1 && isOp(token[0])) {
            if (st.isVoid()) throw runtime_error("Not enough operands");
            int rhs = st.remove();
            if (st.isVoid()) throw runtime_error("Not enough operands");
            int lhs = st.remove();
            switch (token[0]) {
                case '+': st.add(lhs + rhs); break;
                case '-': st.add(lhs - rhs); break;
                case '*': st.add(lhs * rhs); break;
                case '/': st.add(lhs / rhs); break;
            }
        } else {
            st.add(stoi(token));
        }
    }
    int result = st.remove();
    if (!st.isVoid()) throw runtime_error("Too many operands");
    return result;
}
