// Copyright 2025 NNTU-CS
#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include "tstack.h"

static int precedence(char op) {
    return (op == '+' || op == '-') ? 1 : (op == '*' || op == '/') ? 2 : 0;
}
static inline bool isOp(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}
std::string infx2pstfx(const std::string& inf) {
    const int kStackSize = 128;
    TStack<char, kStackSize> ops;
    std::string out;
    for (size_t i = 0; i < inf.size(); ++i) {
        char ch = inf[i];
        if (std::isspace(static_cast<unsigned char>(ch))) {
            continue;
        }
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            while (i < inf.size() && std::isdigit(static_cast<unsigned
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
int eval(const std::string& post) {
    const int kStackSize = 128;
    TStack<int, kStackSize> st;
    std::istringstream ss(post);
    std::string token;
    while (ss >> token) {
        if (token.size() == 1 && isOp(token[0])) {
            if (st.isVoid()) throw std::runtime_error("Not enough operands");
            int rhs = st.remove();
            if (st.isVoid()) throw std::runtime_error("Not enough operands");
            int lhs = st.remove();
            switch (token[0]) {
                case '+': st.add(lhs + rhs); break;
                case '-': st.add(lhs - rhs); break;
                case '*': st.add(lhs * rhs); break;
                case '/': st.add(lhs / rhs); break;
            }
        } else {
            st.add(std::stoi(token));
        }
    }
    int result = st.remove();
    if (!st.isVoid()) throw std::runtime_error("Too many operands");
    return result;
}
