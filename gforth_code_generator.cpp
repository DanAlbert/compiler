#include "gforth_code_generator.h"

#include <assert.h>

#include "messages.h"

GforthCodeGenerator::GforthCodeGenerator(const SyntaxNode* tree, FILE* out) :
	CodeGenerator(tree, out)
{
}

void GforthCodeGenerator::Synthesize(void)
{
	assert(this->out);
	assert(this->tree);

	for (auto it = this->tree->cbegin(); it != this->tree->cend(); ++it)
	{
		this->synthesizeNode(&*it);
	}

	fputc('\n', this->out);
}

void GforthCodeGenerator::synthesizeNode(const SyntaxNode* node)
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

void GforthCodeGenerator::synthesizeList(const SyntaxNode* node)
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

void GforthCodeGenerator::synthesizeSymbol(const SyntaxNode* node)
{
	assert(node);
	assert(this->out);
	assert(node->GetType() == Token::Type::Symbol);
	fprintf(this->out, "%s ", this->equivOf(node->GetToken()).c_str());
}

void GforthCodeGenerator::synthesizeString(const SyntaxNode* node)
{
	assert(node);
	assert(this->out);
	assert(node->GetType() == Token::Type::String);
	fprintf(this->out, "%s ", node->GetValue().c_str());
}

void GforthCodeGenerator::synthesizeNumber(const SyntaxNode* node)
{
	assert(node);
	assert(this->out);
	assert(node->GetType() == Token::Type::Number);
	fprintf(this->out, "%s ", node->GetValue().c_str());
}

void GforthCodeGenerator::handleSyntax(const SyntaxNode* node)
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

std::string GforthCodeGenerator::equivOf(const Token& token) const
{
	switch (token.GetType())
	{
	case Token::Type::Symbol:
		return this->equivSymbol(token.GetLexeme());
	default:
		return token.GetLexeme();
	}
}

std::string GforthCodeGenerator::equivSymbol(const std::string& symbol) const
{
	if (symbol == "display")
	{
		return ".";
	}
	else if (symbol == "newline")
	{
		return "cr";
	}

	return symbol;
}

