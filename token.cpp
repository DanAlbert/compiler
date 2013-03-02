#include "token.h"

#include <assert.h>
#include <stdlib.h>
#include <sstream>
#include <string>

#include "messages.h"

Token::Token(Type type, std::string lexeme)
{
	this->type = type;
	this->lexeme = lexeme;
}

Token::Token(const Token& token)
{
	this->type = token.type;
	this->lexeme = token.lexeme;
}

Token& Token::operator=(const Token& rhs)
{
	if (this == &rhs)
		return *this;

	this->type = rhs.type;
	this->lexeme = rhs.lexeme;

	return *this;
}

bool Token::operator==(const Token& other) const
{
	return (this->GetType() == other.GetType()) &&
	       (this->GetLexeme() == other.GetLexeme());
}

bool Token::operator!=(const Token& other) const
{
	return !(*this == other);
}

const std::string Token::ToString(void) const
{
	std::ostringstream builder;
	std::string type = TokenTypeToString(this->type);
	builder << '<' << type << ", " << this->lexeme << '>';
	return builder.str();
}

void Token::Print(FILE* file) const
{
	assert(file);
	fprintf(file, "%s", this->ToString().c_str());
}

std::string TokenTypeToString(Token::Type type)
{
	switch (type)
	{
	case Token::Type::Number:
		return std::string("number");
		break;
	case Token::Type::String:
		return std::string("string");
		break;
	case Token::Type::Syntax:
		return std::string("syntax");
		break;
	case Token::Type::Symbol:
		return std::string("symbol");
		break;
	default:
		ERROR("invalid token");
		exit(EXIT_FAILURE);
	}
}

