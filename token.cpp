#include "token.h"

#include <assert.h>
#include <stdlib.h>
#include <string>

#include "messages.h"

std::string TokenTypeToString(TokenType type);

Token::Token(TokenType type, std::string token)
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

std::string TokenTypeToString(TokenType type)
{
	switch (type)
	{
	case TOKEN_NUMBER:
		return std::string("number");
		break;
	case TOKEN_STRING:
		return std::string("string");
		break;
	case TOKEN_SYNTAX:
		return std::string("syntax");
		break;
	case TOKEN_SYMBOL:
		return std::string("symbol");
		break;
	case TOKEN_INVAL:
		// fall through
	default:
		ERROR("invalid token");
		exit(EXIT_FAILURE);
	}
}

