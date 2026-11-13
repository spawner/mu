#pragma once

#include <vector>
#include <string>
#include <format>
#include <utility>
#include <variant>
#include <unordered_map>

using Literal = std::variant<std::monostate, std::string, double>;

enum class TokenType {
	// single char tokens
	LPAREN, RPAREN, LBRACE, RBRACE, 
	COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

	// 
	BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, 
	GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,

	// literals
	IDENTIFIER, STRING, NUMBER,

	// keywords
	AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
	PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

	ENOF
};

struct Token {
public:
	explicit Token(TokenType type, const std::string& lexeme, Literal literal, std::size_t line) : _type(type), _lexeme(lexeme), _literal(std::move(literal)), _line(line) {};

public:
	const std::string toString() const {
		if (_type == TokenType::ENOF) return "EOF";

		std::string lit = std::visit([](const auto& v) -> std::string {
			using T = std::decay_t<decltype(v)>;
			if constexpr (std::is_same_v<T, std::monostate>) return "nil";
			else return std::format("{}", v);
		}, _literal);

		return std::format("type: {}, lexeme: {}, literal: {}", std::to_underlying(_type), _lexeme, lit);
	}

private:
	TokenType _type;
	std::string _lexeme;
	Literal _literal;
	std::size_t _line;
};

class Scanner {
public:
	explicit Scanner(std::string source);
	~Scanner() = default;

public:
	std::vector<Token> scanTokens();

private:
	void scanToken();
	void addToken(TokenType type, Literal literal = {});

	bool match(char expected);
	char peek();
	char peekNext();
	bool atEnd();

	void string();
	void number();
	void identifier();
private:
	std::size_t _start = 0;
	std::size_t _current = 0;
	std::size_t _line = 1;

	std::string _source;
	std::vector<Token> _tokens;
	
	std::unordered_map<std::string, TokenType> keywords = {
		{"and", TokenType::AND},
		{"class", TokenType::CLASS},
		{"else", TokenType::ELSE},
		{"false", TokenType::FALSE},
		{"for", TokenType::FOR},
		{"fun", TokenType::FUN},
		{"if", TokenType::IF},
		{"nil", TokenType::NIL},
		{"or", TokenType::OR},
		{"print", TokenType::PRINT},
		{"return", TokenType::RETURN},
		{"super", TokenType::SUPER},
		{"this", TokenType::THIS},
		{"true", TokenType::TRUE},
		{"var", TokenType::VAR},
		{"while", TokenType::WHILE}
	};
};