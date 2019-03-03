#include "pch.h"
#include "Scanner.h"

#include "Error.h"

#include <string>
#include <map>



std::map<std::string, TokenType> createKeywords()
{
	std::map<std::string, TokenType> keywords;
	keywords.emplace("and", AND);
	keywords.emplace("class", CLASS);
	keywords.emplace("else", ELSE);
	keywords.emplace("false", FALSE);
	keywords.emplace("for", FOR);
	keywords.emplace("fun", FUN);
	keywords.emplace("if", IF);
	keywords.emplace("nil", NIL);
	keywords.emplace("or", OR);
	keywords.emplace("print", PRINT);
	keywords.emplace("return", RETURN);
	keywords.emplace("super", SUPER);
	keywords.emplace("this", THIS);
	keywords.emplace("true", TRUE);
	keywords.emplace("var", VAR);
	keywords.emplace("while", WHILE);
	return keywords;
}
std::map<std::string, TokenType> keywords = createKeywords();

Scanner::Scanner(const std::string& source) : source_(source)
{
}


Scanner::~Scanner()
{
}

bool Scanner::isAtEnd() const
{
	return current_ >= source_.size();
}

void Scanner::scanToken()
{
	char c = advance();
	switch (c) {
	case '(': addToken(LEFT_PAREN); break;
	case ')': addToken(RIGHT_PAREN); break;
	case '{': addToken(LEFT_BRACE); break;
	case '}': addToken(RIGHT_BRACE); break;
	case ',': addToken(COMMA); break;
	case '.': addToken(DOT); break;
	case '-': addToken(MINUS); break;
	case '+': addToken(PLUS); break;
	case ';': addToken(SEMICOLON); break;
	case '*': addToken(STAR); break;
	case '!': addToken(match('=') ? BANG_EQUAL : BANG); break;
	case '=': addToken(match('=') ? EQUAL_EQUAL : EQUAL); break;
	case '<': addToken(match('=') ? LESS_EQUAL : LESS); break;
	case '>': addToken(match('=') ? GREATER_EQUAL : GREATER); break;
	case '/':
		if (match('/'))
		{
			while (peek() != '\n' && !isAtEnd()) advance();
		}
		else
		{
			addToken(SLASH);
		}
		break;
	case ' ':
	case '\r':
	case '\t':
		// Ignore whitespace.                      
		break;

	case '\n':
		line_++;
		break;
	case '"': string(); break;
	default:
		if (isDigit(c))
		{
			number();
		}
		else if (isAlpha(c))
		{
			identifier();
		}
		else
		{
			error(line_, "Unexpected character.");
		}
		break;
	}
}

bool Scanner::isAlpha(char c) const
{
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		c == '_';
}

bool Scanner::isAlphaNumeric(char c) const
{
	return isAlpha(c) || isDigit(c);
}

void Scanner::identifier()
{
	while (isAlphaNumeric(peek())) advance();

	auto text = source_.substr(start_, current_);

	auto it = keywords.find(text);
	TokenType type = IDENTIFIER;
	if (it != keywords.end()) type = it->second;
	addToken(type);
}

bool Scanner::isDigit(char c) const
{
	return c >= '0' && c <= '9';
}

void Scanner::number()
{
	while (isDigit(peek())) advance();

	if (peek() == '.' && isDigit(peekNext()))
	{
		advance();
		while (isDigit(peek())) advance();
	}

	addToken(NUMBER, source_.substr(start_, current_));
}

char Scanner::peekNext() const
{
	if (current_ + 1 >= source_.size()) return '\0';
	return source_[current_ + 1];
}

void Scanner::string()
{
	while (peek() != '"' && !isAtEnd())
	{
		if (peek() == '\n') line_++;
		advance();
	}

	if (isAtEnd())
	{
		error(line_, "Unterminated string");
	}

	advance();

	auto value = source_.substr(start_ + 1, current_ + 1);
	addToken(STRING, value);
}

bool Scanner::match(char expected)
{
	if (isAtEnd()) return false;
	if (source_[current_] != expected) return false;

	current_++;
	return true;
}

char Scanner::peek() const
{
	if (isAtEnd()) return '\0';
	return source_[current_];
}

char Scanner::advance()
{
	return source_[current_++];
}

void Scanner::addToken(TokenType type)
{
	addToken(type, "");
}

void Scanner::addToken(TokenType type, const std::string& literal)
{
	auto lexeme = source_.substr(start_, current_);
	tokens_.push_back(Token(type, lexeme, literal, line_));
}

std::vector<Token> Scanner::scanTokens()
{
	while (!isAtEnd())
	{
		start_ = current_;
		scanToken();
	}
	tokens_.push_back(Token(EOF_T, "", "", line_));
	return tokens_;
}