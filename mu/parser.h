#pragma once

#include "grammar.h"

#include <format>
#include <string>
#include <type_traits>
#include <variant>

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
