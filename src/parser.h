#pragma once

#include "grammar.h"

#include <format>
#include <string>
#include <type_traits>
#include <variant>
#include <stack>
#include <set>
#include <iostream>
#include <expected>

template <class... Fs> struct overloaded : Fs... { using Fs::operator()...; };

inline std::string literalToString(const Literal& literal) {
    return std::visit([](const auto& v) -> std::string {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, std::monostate>) return "nil";
        else return std::format("{}", v);
    }, literal);
}

inline std::string print(const Expr& e) {
    return std::visit(overloaded{
        [](const LiteralExpr& l) { return literalToString(l.value); },
        [](const GroupingExpr& g) { return std::format("(group {})", print(*g.inner)); },
        [](const UnaryExpr& u) { return std::format("({} {})", u.op.lexeme(), print(*u.right)); },
        [](const BinaryExpr& b) { return std::format("({} {} {})", b.op.lexeme(),
                                                       print(*b.left), print(*b.right)); },
        }, e.node);
}

inline std::string printRPN(const Expr& e) {
    return std::visit(overloaded{
        [](const LiteralExpr& l) { return literalToString(l.value); },
        [](const GroupingExpr& g) { return std::format("({})", printRPN(*g.inner)); },
        [](const UnaryExpr& u) { return std::format("{} {}", printRPN(*u.right), u.op.lexeme()); },
        [](const BinaryExpr& b) { return std::format("{} {} {}", printRPN(*b.left), printRPN(*b.right), b.op.lexeme()); },
        }, e.node);
}

inline std::expected<int, std::string> evalRPN(const std::string& rpn) {
    const std::set<char> ops = { '+', '-', '/', '*' };

    std::stack<int> st;
    int start = 0;

    for (int i = 0; i < rpn.length(); ++i) {
        if (ops.contains(rpn[i])) {
            if (st.size() < 2) {
                return std::unexpected("not enough arguments for operation");
            }

            char op = rpn[i];

            int op2 = st.top();
            st.pop();
            int op1 = st.top();
            st.pop();

            int result = 0;
            switch (op) {
            case '-':
                result = op1 - op2;
                break;
            case '+':
                result = op1 + op2;
                break;
            case '*':
                result = op1 * op2;
                break;
            case '/':
                result = op1 / op2;
                break;
            }

            st.push(result);
            start = i + 1;
        } else if (rpn[i] == ' ') {
            if (i > start) {
                st.push(std::stoi(rpn.substr(start, i - start)));
            }
            start = i + 1;
        }
    }

    if (start < rpn.length()) {
        st.push(std::stoi(rpn.substr(start)));
    }

    if (st.size() != 1) {
        return std::unexpected("stack is fucked up");
    }

    return st.top();
}