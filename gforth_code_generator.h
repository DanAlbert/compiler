#ifndef GFORTH_CODE_GENERATOR_H
#define GFORTH_CODE_GENERATOR_H

#include <stdio.h>

#include "code_generator.h"
#include "syntax_tree.h"

class GforthCodeGenerator : public CodeGenerator
{
public:
	GforthCodeGenerator(const SyntaxNode* tree, FILE* out = stdout);

	void Synthesize(void);

private:
	void synthesizeNode(const SyntaxNode* node);
	void synthesizeSymbol(const SyntaxNode* node);
	void synthesizeString(const SyntaxNode* node);
	void synthesizeNumber(const SyntaxNode* node);
	void synthesizeFloat(const SyntaxNode* node);
};

#endif // GFORTH_CODE_GENERATOR_H
