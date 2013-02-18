#include "syntax_tree.h"

SyntaxNode::SyntaxNode(const Token& token, const SyntaxNode* parent) :
	token(token),
	parent(parent)
{
}

inline const Token& SyntaxNode::GetToken(void) const
{
	return this->token;
}

inline const SyntaxNode* SyntaxNode::GetParent(void) const
{
	return this->parent;
}

SyntaxNode& SyntaxNode::AddChild(const Token& token)
{
	this->children.push_back(SyntaxNode(token, this));
	return this->children.back(); // TODO: make this thread safe
}

void SyntaxNode::SetChildren(const std::vector<Token>& tokens)
{
	this->RemoveChildren();

	for (auto it = tokens.begin(); it != tokens.end(); ++it)
	{
		this->AddChild(*it);
	}
}

void SyntaxNode::RemoveChild(const Token& token)
{
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

inline std::vector<SyntaxNode>::const_iterator SyntaxNode::cbegin(void) const
{
	return this->children.cbegin();
}

inline std::vector<SyntaxNode>::const_iterator SyntaxNode::cend(void) const
{
	return this->children.cend();
}

inline std::vector<SyntaxNode>::const_reverse_iterator SyntaxNode::crbegin(void) const
{
	return this->children.crbegin();
}

inline std::vector<SyntaxNode>::const_reverse_iterator SyntaxNode::crend(void) const
{
	return this->children.crend();
}

