#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "syntax_tree.h"

class Parser
{
public:
	Parser(const char* file);
	~Parser(void);

	inline const SyntaxNode* GetTree(void) const
	{
		return this->root;
	}

	void ParseTree(void);
	void PrintTree(FILE* file = stdout);

private:
	SyntaxNode* root;
	Lexer lex;

	void expect(Token, const char*);
	void F(SyntaxNode* parent);
	void T(SyntaxNode* parent);
	void S(SyntaxNode* parent);
};

#endif // PARSER_H

