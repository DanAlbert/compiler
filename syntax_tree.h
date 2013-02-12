/**
 * @file syntax_tree.h
 *
 * @author Dan Albert <dan@gingerhq.net>
 */
#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include <vector>

#include "token.h"

class SyntaxTree
{
public:
	class Node
	{
	public:
		Node(const Token& token, const Node* parent = NULL);

		inline const Token& GetToken(void) const;
		inline const Node* GetParent(void) const;

		void AddChild(const Token& token);
		void SetChildren(const std::vector<Token>& tokens);
		void RemoveChild(const Token& token);
		void RemoveChildren(void);

		inline std::vector<Node>::iterator begin(void);
		inline std::vector<Node>::iterator end(void);
		inline std::vector<Node>::reverse_iterator rbegin(void);
		inline std::vector<Node>::reverse_iterator rend(void);
		inline std::vector<Node>::const_iterator cbegin(void) const;
		inline std::vector<Node>::const_iterator cend(void) const;
		inline std::vector<Node>::const_reverse_iterator crbegin(void) const;
		inline std::vector<Node>::const_reverse_iterator crend(void) const;

	private:
		std::vector<Node> children;
		Token token;
		const Node* parent;
	};
	
	SyntaxTree(const Token& token);
	inline const Node& GetRoot(void) const;

	inline std::vector<Node>::iterator begin(void);
	inline std::vector<Node>::iterator end(void);
	inline std::vector<Node>::reverse_iterator rbegin(void);
	inline std::vector<Node>::reverse_iterator rend(void);
	inline std::vector<Node>::const_iterator cbegin(void) const;
	inline std::vector<Node>::const_iterator cend(void) const;
	inline std::vector<Node>::const_reverse_iterator crbegin(void) const;
	inline std::vector<Node>::const_reverse_iterator crend(void) const;

private:
	Node root;
};

#endif // SYNTAX_TREE_H

