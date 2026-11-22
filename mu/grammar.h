#pragma once

#include <memory>
#include <variant>

#include "scanner.h"
/*
* expression -> literal | unary | binary | grouping
* literal -> number | string | "true" | "false" | nil
* grouping -> "(" expression ")"
* binary -> expression operator expression
* unary -> ('-' | '!') expression
* operator -> '==' | '!=' | '<=' | '>=' | '<' | '>' | '+' | '-' | '*' | '/'
*/


struct Expr;
using ExprPtr = std::unique_ptr<Expr>;

struct LiteralExpr { Literal value; };
struct GroupingExpr { ExprPtr inner; };
struct UnaryExpr { Token op; ExprPtr right; };
struct BinaryExpr { ExprPtr left; ExprPtr right; Token op; };

struct Expr {
	std::variant<LiteralExpr, GroupingExpr, UnaryExpr, BinaryExpr> node;
};

template <typename T, typename...  Args>
ExprPtr makeExpr(Args&&... args) {
	return std::make_unique<Expr>(Expr{ T{std::forward<Args>(args)... } });
}