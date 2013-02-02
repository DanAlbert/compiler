#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>
#include <queue>

#include "token.h"

class Lexer
{
public:
	static const char SYNTAX[];

	Lexer(void);
	~Lexer(void) throw();

	bool Init(const char* file);

	bool HasNext(void);
	const Token Next(void);

private:
	FILE* file;
	char current;

	void getNext(void);
	void findNext(void);
	void finishToken(void);
	inline void match(void) { getNext(); }

	const Token parseNumber(void);
	const Token parseString(void);
	const Token parseSyntax(void);
	const Token parseSymbol(void);
};

#endif // LEXER_H

