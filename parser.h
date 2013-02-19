#ifndef PARSER_H
#define PARSER_H
#include "syntax_tree.h"

class Parser
{
public:
	Parser(void);

	void F(void);
	const SyntaxNode& T(void);
	const SyntaxNode& S(void);

private:
	SyntaxNode tree;
};

#endif // PARSER_H

