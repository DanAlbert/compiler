#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <string>

enum TokenType
{
	TOKEN_INVAL,
	TOKEN_NUMBER,
	TOKEN_STRING,
	TOKEN_SYNTAX,
	TOKEN_SYMBOL
};

class Token
{
public:
	Token(TokenType type, std::string token);
	
	void Print(FILE* file = stdout) const;

private:
	TokenType type;
	std::string token;
};

#endif // TOKEN_H

