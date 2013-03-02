#include "gforth_code_generator.h"

#include <assert.h>

#include "messages.h"

GForthCodeGenerator::GForthCodeGenerator(const SyntaxNode* tree, FILE* out) :
	CodeGenerator(tree, out)
{
}

void GForthCodeGenerator::Synthesize(void)
{
	assert(this->out);
	assert(this->tree);

	for (auto it = this->tree->cbegin(); it != this->tree->cend(); ++it)
	{
		this->synthesizeNode(&*it);
	}

	fputc('\n', this->out);
}

void GForthCodeGenerator::synthesizeNode(const SyntaxNode* node)
{
	assert(node);

	if (node->GetType() == Token::Type::Syntax)
	{
		this->handleSyntax(node);
	}
	else if (node->GetType() == Token::Type::Symbol)
	{
		this->synthesizeSymbol(node);
	}
	else if (node->GetType() == Token::Type::String)
	{
		this->synthesizeString(node);
	}
	else if (node->GetType() == Token::Type::Number)
	{
		this->synthesizeNumber(node);
	}
	else
	{
		CRITICAL("unhandled syntax node type %s",
				 TokenTypeToString(node->GetType()).c_str());
		exit(EXIT_FAILURE);
	}
}

void GForthCodeGenerator::synthesizeList(const SyntaxNode* node)
{
	assert(node);
	assert(node->GetType() == Token::Type::Syntax);
	auto it = node->cbegin();
	const SyntaxNode* first = &*it;
	for (++it; it != node->cend(); ++it)
	{
		this->synthesizeNode(&*it);
	}

	this->synthesizeNode(first);
}

void GForthCodeGenerator::synthesizeSymbol(const SyntaxNode* node)
{
	assert(node);
	assert(this->out);
	assert(node->GetType() == Token::Type::Symbol);
	fprintf(this->out, "%s ", node->GetValue().c_str());
}

void GForthCodeGenerator::synthesizeString(const SyntaxNode* node)
{
	assert(node);
	assert(this->out);
	assert(node->GetType() == Token::Type::String);
	fprintf(this->out, "%s ", node->GetValue().c_str());
}

void GForthCodeGenerator::synthesizeNumber(const SyntaxNode* node)
{
	assert(node);
	assert(this->out);
	assert(node->GetType() == Token::Type::Number);
	fprintf(this->out, "%s ", node->GetValue().c_str());
}

void GForthCodeGenerator::handleSyntax(const SyntaxNode* node)
{
	assert(node->GetType() == Token::Type::Syntax);
	if (node->GetValue() == "(")
	{
		this->synthesizeList(node);
	}
	else if (node->GetValue() == ")")
	{
		return;
	}
	else
	{
		CRITICAL("invalid syntax %s", node->GetValue().c_str());
		exit(EXIT_FAILURE);
	}
}

