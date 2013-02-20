#include "syntax_tree.h"

#include <assert.h>

SyntaxNode::SyntaxNode(const Token& token, const SyntaxNode* parent) :
	token(token),
	parent(parent)
{
}

SyntaxNode::SyntaxNode(const SyntaxNode& other) :
	token(other.token),
	parent(other.parent)
{
	for (auto it = other.cbegin(); it != other.cend(); ++it)
	{
		this->AddChild(*it);
	}
}

inline const Token& SyntaxNode::GetToken(void) const
{
	return this->token;
}

inline const SyntaxNode* SyntaxNode::GetParent(void) const
{
	return this->parent;
}

void SyntaxNode::Print(FILE* file, unsigned int level) const
{
	assert(file);
	for (unsigned int i = 0; i < level; i++)
		fputc('\t', file);

	this->GetToken().Print(file);
	fprintf(file, "\n");
	for (auto it = this->children.cbegin(); it != this->children.cend(); ++it)
	{
		it->Print(file, level + 1);
	}
}

SyntaxNode* SyntaxNode::AddChild(const Token& token)
{
	this->children.push_back(SyntaxNode(token, this));
	return &this->children.back(); // TODO: make this thread safe
}

SyntaxNode* SyntaxNode::AddChild(const SyntaxNode& node)
{
	this->children.push_back(SyntaxNode(node));
	return &this->children.back(); // TODO: make this thread safe
}

void SyntaxNode::SetChildren(const std::vector<SyntaxNode>& children)
{
	this->RemoveChildren();
	this->children = children;
}

void SyntaxNode::RemoveChild(const Token& token)
{
	// TODO: should probably assert that the token is a child of the node
	for (auto it = this->children.begin(); it != this->children.end(); ++it)
	{
		if (it->GetToken() == token)
		{
			this->children.erase(it);
			return;
		}
	}
}

void SyntaxNode::RemoveChildren(void)
{
	this->children.clear();
}

inline std::vector<SyntaxNode>::iterator SyntaxNode::begin(void)
{
	return this->children.begin();
}

inline std::vector<SyntaxNode>::iterator SyntaxNode::end(void)
{
	return this->children.end();
}

inline std::vector<SyntaxNode>::reverse_iterator SyntaxNode::rbegin(void)
{
	return this->children.rbegin();
}

inline std::vector<SyntaxNode>::reverse_iterator SyntaxNode::rend(void)
{
	return this->children.rend();
}

inline const std::vector<SyntaxNode>::const_iterator SyntaxNode::cbegin(void) const
{
	return this->children.cbegin();
}

inline const std::vector<SyntaxNode>::const_iterator SyntaxNode::cend(void) const
{
	return this->children.cend();
}

inline const std::vector<SyntaxNode>::const_reverse_iterator
SyntaxNode::crbegin(void) const
{
	return this->children.crbegin();
}

inline const std::vector<SyntaxNode>::const_reverse_iterator
SyntaxNode::crend(void) const
{
	return this->children.crend();
}

