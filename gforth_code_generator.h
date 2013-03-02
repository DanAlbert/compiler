#ifndef GFORTH_CODE_GENERATOR_H
#define GFORTH_CODE_GENERATOR_H

#include <stdio.h>

#include "code_generator.h"
#include "syntax_tree.h"

class GForthCodeGenerator : public CodeGenerator
{
public:
	GForthCodeGenerator(const SyntaxNode* tree, FILE* out = stdout);

	void Synthesize(void);

private:
	void synthesizeNode(const SyntaxNode* node);
	void synthesizeList(const SyntaxNode* node);
	void synthesizeSymbol(const SyntaxNode* node);
	void synthesizeString(const SyntaxNode* node);
	void synthesizeNumber(const SyntaxNode* node);

	void handleSyntax(const SyntaxNode* node);
};

#endif // GFORTH_CODE_GENERATOR_H
