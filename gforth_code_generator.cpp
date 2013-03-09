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
		CRITICAL("invalid syntax");
		exit(EXIT_FAILURE);
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
	else if (node->GetType() == Token::Type::Float)
	{
		this->synthesizeFloat(node);
	}
	else
	{
		CRITICAL("unhandled syntax node type %s",
				 TokenTypeToString(node->GetType()).c_str());
		exit(EXIT_FAILURE);
	}
}

void GforthCodeGenerator::synthesizeSymbol(const SyntaxNode* node)
{
	assert(node);
	assert(this->out);
	assert(node->GetType() == Token::Type::Symbol);

	for (auto it = node->cbegin(); it != node->cend(); ++it)
	{
		this->synthesizeNode(&*it);
	}

	fprintf(this->out, "%s ", this->equivOf(node).c_str());
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

void GforthCodeGenerator::synthesizeFloat(const SyntaxNode* node)
{
	assert(node);
	assert(this->out);
	assert(node->GetType() == Token::Type::Float);
	fprintf(this->out, "%se ", node->GetValue().c_str());
}

std::string GforthCodeGenerator::equivOf(const SyntaxNode* node) const
{
	assert(node);

	switch (node->GetType())
	{
	case Token::Type::Symbol:
		return this->equivSymbol(node);
	default:
		return node->GetValue();
	}
}

std::string GforthCodeGenerator::equivSymbol(const SyntaxNode* node) const
{
	std::string symbol = node->GetValue();
	int nparams = node->size();

	if (symbol == "inttofloat")
	{
		return "s>f";
	}
	else if (symbol == "newline")
	{
		return "cr";
	}
	else if (symbol == "%")
	{
		return "mod";
	}
	else if (symbol == "not")
	{
		return "invert";
	}
	else if (symbol == "iff")
	{
		return "=";
	}
	if (node->IsFloat())
	{
		if (symbol == "println")
		{
			return "f. CR";
		}
		else if (symbol == "+")
		{
			return "f+";
		}
		else if (symbol == "-")
		{
			if (nparams == 1)
			{
				return "fnegate";
			}
			else if (nparams == 2)
			{
				return "f-";
			}
			else
			{
				ERROR("wrong number of arguments to %s", symbol.c_str());
				exit(EXIT_FAILURE);
			}
		}
		else if (symbol == "negate")
		{
			return "fnegate";
		}
		else if (symbol == "*")
		{
			return "f*";
		}
		else if (symbol == "/")
		{
			return "f/";
		}
		else if (symbol == "exp")
		{
			return "fexp";
		}
		else if (symbol == "expt")
		{
			return "f**";
		}
		else if (symbol == "^")
		{
			return "f**";
		}
		else if (symbol == "<")
		{
			return "f<";
		}
		else if (symbol == "=")
		{
			return "f=";
		}
		else if (symbol == "sin")
		{
			return "fsin";
		}
		else if (symbol == "cos")
		{
			return "fcos";
		}
		else if (symbol == "tan")
		{
			return "ftan";
		}
	}
	else
	{
		if (symbol == "println")
		{
			return ". CR";
		}
		else if (symbol == "remainder")
		{
			return "mod";
		}
		else if (symbol == "-")
		{
			if (nparams == 1)
			{
				return "negate";
			}
			else if (nparams == 2)
			{
				return "-";
			}
			else
			{
				ERROR("wrong number of arguments to %s", symbol.c_str());
				exit(EXIT_FAILURE);
			}
		}
	}

	return symbol;
}

