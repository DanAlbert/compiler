#include "token.h"

#include <assert.h>
#include <stdlib.h>
#include <string>

#include "messages.h"

std::string TokenTypeToString(Token::Type type);

Token::Token(Type type, std::string token)
{
	this->type = type;
	this->token = token;
}

void Token::Print(FILE* file) const
{
	assert(file);
	fprintf(file, "<%s, %s>",
		   	TokenTypeToString(this->type).c_str(), this->token.c_str());
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

