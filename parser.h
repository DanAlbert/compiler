
#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "syntax_tree.h"

class Parser {
	public:
		Parser(const char* file);
		~Parser();
        void expect(Token, const char*);
		SyntaxNode F();
		SyntaxNode T();
		SyntaxNode S();
		void ParseTree();
		void PrintTree();

    private:
        SyntaxNode *root;
		void PrintHelper( SyntaxNode&, int);
        Lexer lex;
};

#endif // PARSER_H
