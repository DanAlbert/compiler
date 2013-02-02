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
		return this->parse_number();
	}
	else if (this->current == '\'' || this->current == '"')
	{
		return this->parse_string();
	}
	else if (strchr(Lexer::OPERATORS, this->current))
	{
		return this->parse_operator();
	}
	else if (strchr(Lexer::SYNTAX, this->current))
	{
		return this->parse_syntax();
	}
	else if (isalpha(this->current) || this->current == '_')
	{
		return this->parse_id();
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

	this->find_next();
	return true;
}

void Lexer::get_next(void)
{
	assert(this->file);
	this->current = fgetc(this->file);
}

void Lexer::find_next(void)
{
	this->get_next();

	while (isspace(this->current) || this->current == '#')
	{
		if (isspace(this->current))
		{
			while (isspace(this->current)) // skip white space
				this->get_next();
		}
		else if (this->current == '#') // skip comments
		{
			while (this->current != '\n')
				this->get_next();
		}
	}
}

void Lexer::finish_token(void)
{
	if (isspace(this->current) || this->current == '#')
		this->find_next();
}

const Token Lexer::parse_number(void)
{
	std::string token;
	while (isdigit(this->current))
	{
		token += this->current;
		this->match();
	}

	this->finish_token();
	return Token(TOKEN_NUMBER, token);
}

const Token Lexer::parse_string(void)
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

	this->finish_token();
	return Token(TOKEN_STRING, token);
}

const Token Lexer::parse_operator(void)
{
	std::string token;
	token += this->current;
	this->match();
	this->finish_token();
	return Token(TOKEN_OPERATOR,token);
}

const Token Lexer::parse_syntax(void)
{
	std::string token;
	token += this->current;
	this->match();
	this->finish_token();
	return Token(TOKEN_SYNTAX, token);
}

const Token Lexer::parse_id(void)
{
	std::string token;
	while (isalnum(this->current) || this->current == '_')
	{
		token += this->current;
		this->match();
	}

	this->finish_token();
	return Token(TOKEN_ID, token);
}

