#include "scanner.h"


#include "mu.h"

Scanner::Scanner(std::string source) : _source(std::move(source)) {};

std::vector<Token> Scanner::scanTokens() {
	while (!atEnd()) {
		_start = _current;
		scanToken();
	}

	_tokens.push_back(Token(TokenType::ENOF, "", {}, _line));
	return _tokens;
}

void Scanner::scanToken() {
	char ch = _source[_current++];

	switch (ch) {
	case '(':
		addToken(TokenType::LPAREN);
		break;
	case ')':
		addToken(TokenType::RPAREN);
		break;
	case '{':
		addToken(TokenType::LBRACE);
		break;
	case '}':
		addToken(TokenType::RBRACE);
		break;
	case ',':
		addToken(TokenType::COMMA);
		break;
	case '.':
		addToken(TokenType::DOT);
		break;
	case '-':
		addToken(TokenType::MINUS);
		break;
	case '+':
		addToken(TokenType::PLUS);
		break;
	case ';':
		addToken(TokenType::SEMICOLON);
		break;
	case '*':
		addToken(TokenType::STAR);
		break;
	case '!':
		addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
		break;
	case '=':
		addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
		break;
	case '<':
		addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
		break;
	case '>':
		addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
		break;
	case '/':
		if (match('/')) {
			while (peek() != '\n' && !atEnd()) ++_current;
		}
		else {
			addToken(TokenType::SLASH);
		}
		break;
	case ' ':
	case '\r':
	case '\t':
		break;

	case '\n':
		_line++;
		break;

	case '"':
		string();
		break;

	default:
		if (std::isdigit(ch)) {
			number();
		} else if (std::isalpha(ch)) {
			identifier();
		} else {
			Mu::error(_line, std::format("unexpected character: {}", ch));
		}
		break;
	}
}

void Scanner::addToken(TokenType type, Literal literal) {
	std::string text = _source.substr(_start, _current - _start);
	_tokens.emplace_back(type, std::move(text), std::move(literal), _line);
}

bool Scanner::match(char expected) {
	if (atEnd()) return false;

	if (_source[_current] != expected) return false;

	++_current;
	return true;
}

char Scanner::peek() {
	if (atEnd()) return '\0';

	return _source[_current];
}

char Scanner::peekNext() {
	if (_current + 1 >= _source.length()) return '\0';

	return _source[_current + 1];
}

bool Scanner::atEnd() {
	return _current >= _source.length();
}

void Scanner::string() {
	while (peek() != '"' && !atEnd()) {
		if (peek() == '\n') ++_line;
		_current++;
	}

	if (atEnd()) {
		Mu::error(_line, "Unterminated string");
		return;
	}
	_current++;

	std::string value = _source.substr(_start + 1, _current - _start - 2);
	addToken(TokenType::STRING, value);
}

void Scanner::number() {
	while (std::isdigit(peek())) _current++;

	if (peek() == '.' && std::isdigit(peekNext())) {
		++_current;

		while (std::isdigit(peek())) _current++;
	}

	std::string value = _source.substr(_start, _current - _start);
	addToken(TokenType::NUMBER, std::stod(value));
}

void Scanner::identifier() {
	while (std::isalnum(peek())) _current++;
	TokenType type = TokenType::IDENTIFIER;

	std::string value = _source.substr(_start, _current - _start);
	if (keywords.contains(value)) {
		type = keywords[value];
	}
	
	addToken(type);
}