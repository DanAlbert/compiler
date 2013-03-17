#include "semantic_analyzer.h"

#include <assert.h>

#include "messages.h"

SemanticAnalyzer::SemanticAnalyzer(const SyntaxNode* tree) :
	tree(NULL),
	sourceTree(tree)
{
}

SemanticAnalyzer::~SemanticAnalyzer(void)
{
	if (this->tree)
	{
		delete this->tree;
		this->tree = NULL;
	}
}

void SemanticAnalyzer::PrintTree(FILE* file)
{
	assert(this->tree);
    this->tree->Print(file);
}

void SemanticAnalyzer::Construct(void)
{
	this->reorganize();
	this->checkTypes();
}

void SemanticAnalyzer::reorganize(void)
{
	assert(this->sourceTree);
	assert(!this->tree);

	INFO("reorganizing syntax tree");

	if ((this->sourceTree->GetType() != Token::Type::Symbol) &&
	    (this->sourceTree->GetValue() != "statements"))
	{
		CRITICAL("invalid syntax tree root");
		exit(EXIT_FAILURE);
	}

	this->tree = new SyntaxNode(Token(Token::Type::Symbol, "statements"));
	for (auto it = sourceTree->cbegin(); it != sourceTree->cend(); ++it)
	{
		if (it->GetValue() == "(")
		{
			this->list(&*it, this->tree);
		}
		else if (it->GetValue() == ")")
		{
		}
		else
		{
			CRITICAL("invalid syntax");
			exit(EXIT_FAILURE);
		}
	}
}

void SemanticAnalyzer::checkTypes(void)
{
	assert(this->tree);

	INFO("checking types");

	for (auto it = this->tree->begin(); it != this->tree->end(); ++it)
	{
		this->checkNodeTypes(&*it);
	}
}

void SemanticAnalyzer::checkNodeTypes(SyntaxNode* node)
{
	assert(node);

	bool needsFloats = node->IsFloat();
	for (auto it = node->begin(); it != node->end(); ++it)
	{
		this->checkNodeTypes(&*it);
		if (it->IsFloat())
			needsFloats = true;
	}

	if (needsFloats)
	{
		for (auto it = node->begin(); it != node->end(); ++it)
		{
			switch (it->GetType())
			{
			case Token::Type::Symbol:
				if (!it->IsFloat())
					this->makeIntToFloat(it, node);
				break;
			case Token::Type::Number:
				this->makeIntToFloat(it, node);
				break;
			case Token::Type::Float:
				break;
			default:
				ERROR("no conversion exists for %s to %s",
				      TokenTypeToString(it->GetType()).c_str(),
					  TokenTypeToString(Token::Type::Float).c_str());
				exit(EXIT_FAILURE);
			}
		}
	}
}

void SemanticAnalyzer::makeIntToFloat(std::vector<SyntaxNode>::iterator it,
                                      SyntaxNode* parent)
{
	assert(parent);

	SyntaxNode node(Token(Token::Type::Symbol, "inttofloat"));
	parent->InsertBetween(it, node);
}

void SemanticAnalyzer::list(const SyntaxNode* node, SyntaxNode* parent)
{
	assert(node);
	assert(parent);
	
	DEBUG("handling list");

	auto it = node->cbegin();
	if (it->GetValue() == "(")
	{
		parent = parent->AddChild(Token(Token::Type::Symbol, "statements"));
		this->list(&*it, parent);
	}
	else
	{
		parent = parent->AddChild(it->GetToken());
	}

	DEBUG("built op");
	for (++it; it != node->cend(); ++it)
	{
		if (it->GetValue() == "(")
		{
			this->list(&*it, parent);
		}
		else if (it->GetValue() != ")")
		{
			DEBUG("adding child to list");
			parent->AddChild(*it);
		}
		// else ignore
	}
}

