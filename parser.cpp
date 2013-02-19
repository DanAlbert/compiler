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
    // eventually delete the entire tree
}

void Parser::ParseTree()
{
    root = new SyntaxNode( F() );
}

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

void Parser::PrintTree()
{
    PrintHelper(*root, 0);
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
	//print out "in F"
    SyntaxNode Fnode( Token(Token::Type::NonTerminal, "F"), NULL);

    while ( lex.HasNext() ) {
       Fnode.AddChild( T() );
    }
    return Fnode;
}


SyntaxNode Parser::T()
{
	//print out "in T"
	SyntaxNode Tnode( Token(Token::Type::NonTerminal, "T"), NULL);
	
	Token t = lex.Next(); //eat opening paren
	expect (t, "(");
	Tnode.AddChild( SyntaxNode(t, &Tnode) );

	Tnode.AddChild( S() );

	t = lex.Next(); //eat closing paren
	expect(t, ")");
	Tnode.AddChild( SyntaxNode(t, &Tnode) );

	return Tnode;
}


SyntaxNode Parser::S()
{
	//print out "in S"
	SyntaxNode Snode( Token(Token::Type::NonTerminal, "S"), NULL);

	Token firstToken = lex.Next();
	// ****************** ==> () || (S) || ()S || (S)S 
	if (firstToken.GetToken() == "(") {
		Snode.AddChild( SyntaxNode(firstToken, &Snode) );  //adds the "(" to the list of children

		Token t = lex.Next();
		// ****************** ==> () || ()S
		if (t.GetToken() == ")") {      
			Snode.AddChild(SyntaxNode(t, &Snode));  //add the ")" to the list of children

            // We now distinguish between () and ()S
			Token t2 = lex.Next();
			if (t2.GetToken() == ")") {  // this is the () case
                lex.PushBack(t2);
			}
            else {                       // this is the ()S case
				Snode.AddChild( S() );
			}
		}
		// ******************  ==> (S) || (S)S
		else { 
            lex.PushBack(t);  // this token belongs to the S production
			Snode.AddChild( S() );

			Token t4 = lex.Next();   //expect a ")"
            expect(t4, ")");
			Snode.AddChild( SyntaxNode(t4, &Snode) );  //add the ")" token

			Token t5 = lex.Next();       // let's see if an S is following
            lex.PushBack(t5);
			if (t5.GetToken() != ")") {  // we *do* have a following S production
				Snode.AddChild( S() );
			}
		}
	} 

	// ******************************  ==> atom || atom S
	else if (firstToken.GetType() == Token::Type::Symbol) { 

		Snode.AddChild(SyntaxNode(firstToken, &Snode));

		Token lookAhead = lex.Next();   //checking if we're followed by an S
        lex.PushBack(lookAhead);

		if (lookAhead.GetToken() != ")") {   // then either a "(" or an atom
			Snode.AddChild( S() );
		}
	} 

    // *****************************  Doesn't match any production
	else {
		fprintf(stderr, "Expected '(' or an symbol, but found '%s'\n",
            firstToken.GetToken().c_str());
		exit(-1);
	}

    return Snode;
}

