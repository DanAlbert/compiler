#include "lexer.h"
#include "syntax_tree.h"
#include "symbol_table.h"
#include "token.h"
#include "messages.h"
#include "parser.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>


/** End Productions ***
 *   F-> <endoffile> || TF
 *   T-> (S)
 *	 S-> () || atom || (S) || ()S || atom S || (S)S
 */

Parser::Parser(const char* file)
   : root(NULL)
{
	if (!lex.Init(file))
	{
		CRITICAL("Could not open file %s", file);
		exit(EXIT_FAILURE);
	}
}

Parser::~Parser()
{
	if (this->root)
	{
		delete this->root;
		this->root = NULL;
	}
}

void Parser::ParseTree()
{
    root = new SyntaxNode( F() );
}
/*
void Parser::PrintHelper(SyntaxNode& node, int indent)
{
    if (indent == -999) printf("oops");

    // Print out:    T [ ( S ) ]
    //
    // To show that this node is "T", and it's children are '(', S, and ')'
    //
    printf("%s  [", node.GetToken().GetToken().c_str() );
    for (int i=0; i<node.children.size(); i++)
        printf("%*s%s ", indent,"", node.children[i].GetToken().GetToken().c_str());
    printf("]\n");

    // Then for every non-terminal child (in this case, "S"), print out
    //
    // their information indented by one level.
    //
    for (int i=0; i<node.children.size(); i++) {
        if (node.children[i].GetToken().GetType() == Token::Type::NonTerminal) 
           PrintHelper(node.children[i], indent+4);
    }
}
*/

void Parser::PrintTree(FILE* file)
{
	assert(this->root);
    this->root->Print(file);
}

void Parser::expect(Token t, const char *expect_string)
{
	if (t.GetToken() != expect_string) {
		fprintf(stderr, "Expecting string '%s' but encountered '%s'\n",
            expect_string, t.GetToken().c_str());
		exit (-1);
	}
}

SyntaxNode Parser::F()
{
	fprintf(stderr, "In F\n");//DEBUG
    SyntaxNode Fnode( Token(Token::Type::NonTerminal, "F"), NULL);

    while ( lex.HasNext() ) {
	   fprintf(stderr, "In F, about to call T\n"); //DEBUG
       Fnode.AddChild( T() );
    }
    return Fnode;
}


SyntaxNode Parser::T()
{
	SyntaxNode Tnode( Token(Token::Type::NonTerminal, "T"), NULL);
	
	Token t = lex.Next(); //eat opening paren
	fprintf(stderr, "In T, Token: %s\n", t.ToString().c_str());//DEBUG
	expect (t, "(");
	Tnode.AddChild( SyntaxNode(t, &Tnode) );

	Tnode.AddChild( S() );

	t = lex.Next(); //eat closing paren
	fprintf(stderr, "In T, Token: %s\n", t.ToString().c_str());//DEBUG
	expect(t, ")");
	Tnode.AddChild( SyntaxNode(t, &Tnode) );

	return Tnode;
}


SyntaxNode Parser::S()
{
	SyntaxNode Snode( Token(Token::Type::NonTerminal, "S"), NULL);

	Token firstToken = lex.Next();
	fprintf(stderr, "#1 In S, Token: %s\n", firstToken.ToString().c_str());//DEBUG
	// ****************** ==> () || (S) || ()S || (S)S 
	if (firstToken.GetToken() == "(") {
		Snode.AddChild( SyntaxNode(firstToken, &Snode) );  //adds the "(" to the list of children

		Token t = lex.Next();
		fprintf(stderr, "#2 In S, Token: %s\n", t.ToString().c_str()); //DEBUG
		// ****************** ==> () || ()S
		if (t.GetToken() == ")") {      
			Snode.AddChild(SyntaxNode(t, &Snode));  //add the ")" to the list of children

            // We now distinguish between () and ()S
			Token t2 = lex.Next();
            lex.PushBack(t2);
			fprintf(stderr, "#3 In S --> () | ()S, Token: %s\n", t2.ToString().c_str()); //DEBUG
			if (t2.GetToken() == ")") {  // this is the () case
			    fprintf(stderr, "#4 In S --> (), Token: %s\n", t2.ToString().c_str()); //DEBUG
			}
            else {                       // this is the ()S case
			    fprintf(stderr, "#5 In S --> ()S, Token: %s\n", t2.ToString().c_str()); //DEBUG
				Snode.AddChild( S() );
			}
		}
		// ******************  ==> (S) || (S)S
		else { 
            lex.PushBack(t);  // this token belongs to the S production
			Snode.AddChild( S() );

			Token t3 = lex.Next();   //expect a ")"
			fprintf(stderr, "#6 In S -> (S) || (S)S, Token: %s\n", t3.ToString().c_str()); //DEBUG
            expect(t3, ")");
			Snode.AddChild( SyntaxNode(t3, &Snode) );  //add the ")" token

			Token t4 = lex.Next();       // let's see if an S is following
            lex.PushBack(t4);
			fprintf(stderr, "#7 In S, lookahead Token: %s\n", t4.ToString().c_str()); //DEBUG
			if (t4.GetToken() != ")") {  // we *do* have a following S production
			    fprintf(stderr, "#8 In S\n"); //DEBUG
				Snode.AddChild( S() );
			}
		}
	} 

	// ******************************  ==> atom || atom S
	else if (  firstToken.GetType() == Token::Type::Symbol 
			|| firstToken.GetType() == Token::Type::Number) { 

		Snode.AddChild(SyntaxNode(firstToken, &Snode));

		Token lookAhead = lex.Next();   //checking if we're followed by an S
		fprintf(stderr, "#9 In S, Token: %s\n", lookAhead.ToString().c_str()); //DEBUG
        lex.PushBack(lookAhead);

		if (lookAhead.GetToken() != ")") {   // then either a "(" or an atom
		    fprintf(stderr, "#10 In S\n"); //DEBUG
			Snode.AddChild( S() );
		}
	} 

    // *****************************  Doesn't match any production
	else {
		fprintf(stderr, "Expected '(' or a symbol, but found '%s'\n",
            firstToken.GetToken().c_str());
		exit(-1);
	}

    return Snode;
}

