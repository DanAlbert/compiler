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

	inline const Token& GetToken(void) const;
	inline const SyntaxNode* GetParent(void) const;

	SyntaxNode& AddChild(const Token& token);
	void SetChildren(const std::vector<Token>& tokens);
	void RemoveChild(const Token& token);
	void RemoveChildren(void);

	inline std::vector<SyntaxNode>::iterator begin(void);
	inline std::vector<SyntaxNode>::iterator end(void);
	inline std::vector<SyntaxNode>::reverse_iterator rbegin(void);
	inline std::vector<SyntaxNode>::reverse_iterator rend(void);
	inline std::vector<SyntaxNode>::const_iterator cbegin(void) const;
	inline std::vector<SyntaxNode>::const_iterator cend(void) const;
	inline std::vector<SyntaxNode>::const_reverse_iterator crbegin(void) const;
	inline std::vector<SyntaxNode>::const_reverse_iterator crend(void) const;

private:
	std::vector<SyntaxNode> children;
	Token token;
	const SyntaxNode* parent;
};

typedef SyntaxNode SyntaxTree;

#endif // SYNTAX_TREE_H

