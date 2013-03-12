#include "gforth_code_generator.h"

#include <assert.h>
#include <functional>
#include <sstream>
#include <unordered_map>

#include "messages.h"

#define T_FUNC(block) ([](const std::string& s, int n, bool f) \
		{ (void)s; (void)n; (void)f; {block} })
#define T_RETURN(val) T_FUNC(return val;)

typedef std::function<std::string(const std::string&, int, bool)>
		TranslationFunction;

std::string mixedOp(const std::string& symbol, int nparams, bool isFloat);

std::unordered_map<std::string, TranslationFunction> translations =
{
	{"+",          mixedOp},
	{"-",          mixedOp},
	{"*",          mixedOp},
	{"/",          mixedOp},
	{"<",          mixedOp},
	{"=",          mixedOp},
	{"println",    mixedOp},
	{"%",          T_RETURN("mod")},
	{"remainder",  T_RETURN("mod")},
	{"^",          T_RETURN("f**")},
	{"expt",       T_RETURN("f**")},
	{"exp",        T_RETURN("fexp")},
	{"sin",        T_RETURN("fsin")},
	{"cos",        T_RETURN("fcos")},
	{"tan",        T_RETURN("ftan")},
	{"iff",        T_RETURN("=")},
	{"not",        T_RETURN("invert")},
	{"newline",    T_RETURN("cr")},
	{"inttofloat", T_RETURN("s>f")},
};

std::string mixedOp(const std::string& symbol, int nparams, bool isFloat)
{
	std::ostringstream builder;
	if (isFloat)
		builder << 'f';

	if ((symbol == "-") && (nparams == 1))
	{
		builder << "negate";
	}
	else if (symbol == "println")
	{
		if (nparams == 1)
		{
			builder << ". cr";
		}
		else
		{
			ERROR("%s does not take %d arguments", symbol.c_str(), nparams);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		builder << symbol;
	}

	return builder.str();
}

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
		CRITICAL("unexpected %s in syntax tree", node->GetValue().c_str());
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

	std::string newSym;

	auto t = translations.find(node->GetValue());
	if (t != translations.end())
	{
		newSym = (t->second)(node->GetValue(), node->size(), node->IsFloat());
	}
	else
	{
		ERROR("no translation exists for %s", node->GetValue().c_str());
		exit(EXIT_FAILURE);
	}

	fprintf(this->out, "%s ", newSym.c_str());
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

