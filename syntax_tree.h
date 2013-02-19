/**
 * @file syntax_tree.h
 *
 * @author Dan Albert <dan@gingerhq.net>
 */
#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include <vector>

#include "token.h"

class SyntaxNode
{
public:
	SyntaxNode(const Token& token, const SyntaxNode* parent = NULL);
	SyntaxNode(const SyntaxNode& other);

	inline const Token& GetToken(void) const;
	inline const SyntaxNode* GetParent(void) const;

	void Print(unsigned int level = 0, FILE* file = stdout) const;

	SyntaxNode& AddChild(const Token& token);
	SyntaxNode& AddChild(const SyntaxNode& node);
	void SetChildren(const std::vector<SyntaxNode>& children);
	void RemoveChild(const Token& token);
	void RemoveChildren(void);

	inline std::vector<SyntaxNode>::iterator begin(void);
	inline std::vector<SyntaxNode>::iterator end(void);
	inline std::vector<SyntaxNode>::reverse_iterator rbegin(void);
	inline std::vector<SyntaxNode>::reverse_iterator rend(void);
	inline const std::vector<SyntaxNode>::const_iterator cbegin(void) const;
	inline const std::vector<SyntaxNode>::const_iterator cend(void) const;

	inline const std::vector<SyntaxNode>::const_reverse_iterator
		crbegin(void) const;

	inline const std::vector<SyntaxNode>::const_reverse_iterator
		crend(void) const;

private:
	std::vector<SyntaxNode> children;
	Token token;
	const SyntaxNode* parent;
};

#endif // SYNTAX_TREE_H

