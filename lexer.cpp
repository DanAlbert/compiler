#include "lexer.h"

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "messages.h"

const char Lexer::OPERATORS[] = "+-*/=<>!&|^~%";
const char Lexer::SYNTAX[] = "(){}[];,:.@";

Lexer::Lexer(void) :
	current('\0')
{
}

Lexer::~Lexer(void) throw()
{
	if (this->file)
	{
		fclose(this->file);
		this->file = NULL;
	}
}

bool Lexer::HasNext(void)
{
	return this->current != EOF;
}

const Token Lexer::Next(void)
{
	assert(this->HasNext());

	if (isdigit(this->current))
	{
		return this->parseNumber();
	}
	else if (this->current == '\'' || this->current == '"')
	{
		return this->parseString();
	}
	else if (strchr(Lexer::OPERATORS, this->current))
	{
		return this->parseOperator();
	}
	else if (strchr(Lexer::SYNTAX, this->current))
	{
		return this->parseSyntax();
	}
	else if (isalpha(this->current) || this->current == '_')
	{
		return this->parseId();
	}
	else
	{
		// TODO: add a line number and file name
		ERROR("invalid token: %c", this->current);
		exit(EXIT_FAILURE); // TODO: this might be a little extreme
	}
}

bool Lexer::Init(const char* file)
{
	assert(file);

	this->file = fopen(file, "r");
	if (!this->file)
		return false;

	this->findNext();
	return true;
}

void Lexer::getNext(void)
{
	assert(this->file);
	this->current = fgetc(this->file);
}

void Lexer::findNext(void)
{
	this->getNext();

	while (isspace(this->current) || this->current == '#')
	{
		if (isspace(this->current))
		{
			while (isspace(this->current)) // skip white space
				this->getNext();
		}
		else if (this->current == '#') // skip comments
		{
			while (this->current != '\n')
				this->getNext();
		}
	}
}

void Lexer::finishToken(void)
{
	if (isspace(this->current) || this->current == '#')
		this->findNext();
}

const Token Lexer::parseNumber(void)
{
	std::string token;
	while (isdigit(this->current))
	{
		token += this->current;
		this->match();
	}

	this->finishToken();
	return Token(TOKEN_NUMBER, token);
}

const Token Lexer::parseString(void)
{
	std::string token;
	char quote_type = this->current;
	this->match();
	while (this->HasNext() && this->current == quote_type)
	{
		token += this->current;
		this->match();
	}

	if (this->HasNext())
	{
		this->match();
	}
	else
	{
		ERROR("Unterminated string value");
		exit(EXIT_FAILURE);
	}

	this->finishToken();
	return Token(TOKEN_STRING, token);
}

const Token Lexer::parseOperator(void)
{
	std::string token;
	token += this->current;
	this->match();
	this->finishToken();
	return Token(TOKEN_OPERATOR,token);
}

const Token Lexer::parseSyntax(void)
{
	std::string token;
	token += this->current;
	this->match();
	this->finishToken();
	return Token(TOKEN_SYNTAX, token);
}

const Token Lexer::parseId(void)
{
	std::string token;
	while (isalnum(this->current) || this->current == '_')
	{
		token += this->current;
		this->match();
	}

	this->finishToken();
	return Token(TOKEN_ID, token);
}

