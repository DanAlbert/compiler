#include "gforth_code_generator.h"

#include <assert.h>
#include <sstream>

#include "messages.h"

std::unordered_map<std::string, std::string> symbolTranslations =
{
	{"%",          "mod"},
	{"remainder",  "mod"},
	{"^",          "f**"},
	{"expt",       "f**"},
	{"exp",        "fexp"},
	{"sin",        "fsin"},
	{"cos",        "fcos"},
	{"tan",        "ftan"},
	{"iff",        "="},
	{"not",        "invert"},
	{"newline",    "cr"},
	{"inttofloat", "s>f"},
};

void GforthCodeGenerator::translatedOp(const SyntaxNode* node)
{
	assert(this->out);
	assert(node);

	for (auto it = node->cbegin(); it != node->cend(); ++it) \
	{
		this->synthesizeNode(&*it);
	}

	auto t = symbolTranslations.find(node->GetValue());
	if (t != symbolTranslations.end())
	{
		fprintf(this->out, "%s ", t->second.c_str());
	}
	else
	{
		ERROR("no translation exists for %s", node->GetValue().c_str());
		exit(EXIT_FAILURE);
	}
}

void GforthCodeGenerator::mixedOp(const SyntaxNode* node)
{
	assert(this->out);
	assert(node);

	for (auto it = node->cbegin(); it != node->cend(); ++it)
	{
		this->synthesizeNode(&*it);
	}

	std::string symbol = node->GetValue();
	int nparams = node->size();

	std::ostringstream builder;
	if (node->IsFloat())
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

	fprintf(this->out, "%s ", builder.str().c_str());
}

void GforthCodeGenerator::ifOp(const SyntaxNode* node)
{
	assert(this->out);
	assert(node);

	int i = 0;
	const SyntaxNode* condition = NULL;
	const SyntaxNode* thenBody = NULL;
	const SyntaxNode* elseBody = NULL;

	for (auto it = node->cbegin(); it != node->cend(); ++it)
	{
		switch (i++)
		{
		case 0:
			condition = &*it;
			break;
		case 1:
			thenBody = &*it;
			break;
		case 2:
			elseBody = &*it;
			break;
		default:
			ERROR("too many arguments to if");
			exit(EXIT_FAILURE);
		}
	}

	if (!condition || !thenBody)
	{
		ERROR("not enough arguments to if");
		exit(EXIT_FAILURE);
	}

	this->synthesizeNode(condition);
	fprintf(this->out, "if ");
	this->synthesizeNode(thenBody);
	if (elseBody)
	{
		fprintf(this->out, "else ");
		this->synthesizeNode(elseBody);
	}
	fprintf(this->out, "endif ");
}

GforthCodeGenerator::GforthCodeGenerator(const SyntaxNode* tree, FILE* out) :
	CodeGenerator(tree, out)
{
	this->addTranslation("+",          &GforthCodeGenerator::mixedOp);
	this->addTranslation("-",          &GforthCodeGenerator::mixedOp);
	this->addTranslation("*",          &GforthCodeGenerator::mixedOp);
	this->addTranslation("/",          &GforthCodeGenerator::mixedOp);
	this->addTranslation("<",          &GforthCodeGenerator::mixedOp);
	this->addTranslation("=",          &GforthCodeGenerator::mixedOp);
	this->addTranslation("println",    &GforthCodeGenerator::mixedOp);
	this->addTranslation("%",          &GforthCodeGenerator::translatedOp);
	this->addTranslation("remainder",  &GforthCodeGenerator::translatedOp);
	this->addTranslation("^",          &GforthCodeGenerator::translatedOp);
	this->addTranslation("expt",       &GforthCodeGenerator::translatedOp);
	this->addTranslation("exp",        &GforthCodeGenerator::translatedOp);
	this->addTranslation("sin",        &GforthCodeGenerator::translatedOp);
	this->addTranslation("cos",        &GforthCodeGenerator::translatedOp);
	this->addTranslation("tan",        &GforthCodeGenerator::translatedOp);
	this->addTranslation("iff",        &GforthCodeGenerator::translatedOp);
	this->addTranslation("not",        &GforthCodeGenerator::translatedOp);
	this->addTranslation("newline",    &GforthCodeGenerator::translatedOp);
	this->addTranslation("inttofloat", &GforthCodeGenerator::translatedOp);
	this->addTranslation("if",         &GforthCodeGenerator::ifOp);
}

void GforthCodeGenerator::addTranslation(const std::string& symbol,
		                                 GforthCodeGenerator::TranslationFunction func)
{
	this->translationFuncs[symbol] = func;
}

void GforthCodeGenerator::Synthesize(void)
{
	assert(this->out);
	assert(this->tree);

	// our entire output needs to be wrapped in a defined word because gforth
	// doesn't allow conditional branches or loops outside of words
	fprintf(this->out, ": __MAIN__ ");
	for (auto it = this->tree->cbegin(); it != this->tree->cend(); ++it)
	{
		this->synthesizeNode(&*it);
	}
	fprintf(this->out, "; __MAIN__");

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

	std::string newSym;

	auto t = this->translationFuncs.find(node->GetValue());
	if (t != this->translationFuncs.end())
	{
		(t->second)(*this, node);
	}
	else
	{
		ERROR("no translation exists for %s", node->GetValue().c_str());
		exit(EXIT_FAILURE);
	}
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

