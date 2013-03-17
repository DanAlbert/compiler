#ifndef GFORTH_CODE_GENERATOR_H
#define GFORTH_CODE_GENERATOR_H

#include <stdio.h>

#include <functional>
#include <unordered_map>

#include "code_generator.h"
#include "syntax_tree.h"

class GforthCodeGenerator : public CodeGenerator
{
public:
	typedef std::function<void(GforthCodeGenerator&,
	                           const SyntaxNode* node)> TranslationFunction;

	GforthCodeGenerator(const SyntaxNode* tree, FILE* out = stdout);

	void Synthesize(void);

private:
	std::unordered_map<std::string, TranslationFunction> translationFuncs;

	void addTranslation(const std::string& symbol, TranslationFunction);

	void synthesizeNode(const SyntaxNode* node);
	void synthesizeSymbol(const SyntaxNode* node);
	void synthesizeString(const SyntaxNode* node);
	void synthesizeNumber(const SyntaxNode* node);
	void synthesizeFloat(const SyntaxNode* node);

	void translatedOp(const SyntaxNode* node);
	void mixedOp(const SyntaxNode* node);
	void ifOp(const SyntaxNode* node);
};

#endif // GFORTH_CODE_GENERATOR_H

