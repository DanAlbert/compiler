#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "syntax_tree.h"

class Parser
{
public:
	Parser(const char* file);
	~Parser(void);
	void expect(Token, const char*);
	SyntaxNode F(void);
	SyntaxNode T(void);
	SyntaxNode S(void);
	void ParseTree(void);
	void PrintTree(FILE* file = stdout);

private:
	SyntaxNode* root;
	Lexer lex;
};

#endif // PARSER_H

