#include "syntax_tree.h"

SyntaxTree::Node::Node(const Token& token, const Node* parent) :
	token(token),
	parent(parent)
{
}

inline const Token& SyntaxTree::Node::GetToken(void) const
{
	return this->token;
}

inline const SyntaxTree::Node* SyntaxTree::Node::GetParent(void) const
{
	return this->parent;
}

void SyntaxTree::Node::AddChild(const Token& token)
{
	this->children.push_back(Node(token, this));
}

void SyntaxTree::Node::SetChildren(const std::vector<Token>& tokens)
{
	this->RemoveChildren();

	for (auto it = tokens.begin(); it != tokens.end(); ++it)
	{
		this->AddChild(*it);
	}
}

void SyntaxTree::Node::RemoveChild(const Token& token)
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

void SyntaxTree::Node::RemoveChildren(void)
{
	this->children.clear();
}

inline std::vector<SyntaxTree::Node>::iterator SyntaxTree::Node::begin(void)
{
	return this->children.begin();
}

inline std::vector<SyntaxTree::Node>::iterator SyntaxTree::Node::end(void)
{
	return this->children.end();
}

inline std::vector<SyntaxTree::Node>::reverse_iterator SyntaxTree::Node::rbegin(void)
{
	return this->children.rbegin();
}

inline std::vector<SyntaxTree::Node>::reverse_iterator SyntaxTree::Node::rend(void)
{
	return this->children.rend();
}

inline std::vector<SyntaxTree::Node>::const_iterator SyntaxTree::Node::cbegin(void) const
{
	return this->children.cbegin();
}

inline std::vector<SyntaxTree::Node>::const_iterator SyntaxTree::Node::cend(void) const
{
	return this->children.cend();
}

inline std::vector<SyntaxTree::Node>::const_reverse_iterator SyntaxTree::Node::crbegin(void) const
{
	return this->children.crbegin();
}

inline std::vector<SyntaxTree::Node>::const_reverse_iterator SyntaxTree::Node::crend(void) const
{
	return this->children.crend();
}

SyntaxTree::SyntaxTree(const Token& token) :
	root(token)
{
}

inline const SyntaxTree::Node& SyntaxTree::GetRoot(void) const
{
	return this->root;
}

inline std::vector<SyntaxTree::Node>::iterator SyntaxTree::begin(void)
{
	return this->root.begin();
}

inline std::vector<SyntaxTree::Node>::iterator SyntaxTree::end(void)
{
	return this->root.end();
}

inline std::vector<SyntaxTree::Node>::reverse_iterator SyntaxTree::rbegin(void)
{
	return this->root.rbegin();
}

inline std::vector<SyntaxTree::Node>::reverse_iterator SyntaxTree::rend(void)
{
	return this->root.rend();
}

inline std::vector<SyntaxTree::Node>::const_iterator SyntaxTree::cbegin(void) const
{
	return this->GetRoot().cbegin();
}

inline std::vector<SyntaxTree::Node>::const_iterator SyntaxTree::cend(void) const
{
	return this->GetRoot().cend();
}

inline std::vector<SyntaxTree::Node>::const_reverse_iterator SyntaxTree::crbegin(void) const
{
	return this->GetRoot().crbegin();
}

inline std::vector<SyntaxTree::Node>::const_reverse_iterator SyntaxTree::crend(void) const
{
	return this->GetRoot().crend();
}

