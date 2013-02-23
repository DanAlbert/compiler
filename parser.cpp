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

void Parser::PrintTree(FILE* file)
{
	assert(this->root);
    this->root->Print(file);
}

void Parser::expect(Token t, const char *expect_string)
{
	if (t.GetToken() != expect_string) {
		fprintf(stderr, "Expecting string '%s' but encountered '%s'",
            expect_string, t.GetToken().c_str());
		exit (-1);
	}
}

SyntaxNode Parser::F()
{
	DEBUG("In F");
    SyntaxNode Fnode( Token(Token::Type::NonTerminal, "F"), NULL);

    while ( lex.HasNext() ) {
	   DEBUG("In F, about to call T"); 
       Fnode.AddChild( T() );
    }
    return Fnode;
}


SyntaxNode Parser::T()
{
	SyntaxNode Tnode( Token(Token::Type::NonTerminal, "T"), NULL);
	
	Token t = lex.Next(); //eat opening paren
	DEBUG("In T, Token: %s", t.ToString().c_str());
	expect (t, "(");
	Tnode.AddChild( SyntaxNode(t, &Tnode) );

	Tnode.AddChild( S() );

	t = lex.Next(); //eat closing paren
	DEBUG("In T, Token: %s", t.ToString().c_str());
	expect(t, ")");
	Tnode.AddChild( SyntaxNode(t, &Tnode) );

	return Tnode;
}


SyntaxNode Parser::S()
{
	SyntaxNode Snode( Token(Token::Type::NonTerminal, "S"), NULL);

	Token firstToken = lex.Next();
	DEBUG("#1 In S, Token: %s", firstToken.ToString().c_str());
	// ****************** ==> () || (S) || ()S || (S)S 
	if (firstToken.GetToken() == "(") {
		Snode.AddChild( SyntaxNode(firstToken, &Snode) );  //adds the "(" to the list of children

		Token t = lex.Next();
		DEBUG("#2 In S, Token: %s", t.ToString().c_str()); 
		// ****************** ==> () || ()S
		if (t.GetToken() == ")") {      
			Snode.AddChild(SyntaxNode(t, &Snode));  //add the ")" to the list of children

            // We now distinguish between () and ()S
			Token t2 = lex.Next();
            lex.PushBack(t2);
			DEBUG("#3 In S --> () | ()S, Token: %s", t2.ToString().c_str()); 
			if (t2.GetToken() == ")") {  // this is the () case
			    DEBUG("#4 In S --> (), Token: %s", t2.ToString().c_str()); 
			}
            else {                       // this is the ()S case
			    DEBUG("#5 In S --> ()S, Token: %s", t2.ToString().c_str());
				Snode.AddChild( S() );
			}
		}
		// ******************  ==> (S) || (S)S
		else { 
            lex.PushBack(t);  // this token belongs to the S production
			Snode.AddChild( S() );

			Token t3 = lex.Next();   //expect a ")"
			DEBUG("#6 In S -> (S) || (S)S, Token: %s", t3.ToString().c_str()); 
            expect(t3, ")");
			Snode.AddChild( SyntaxNode(t3, &Snode) );  //add the ")" token

			Token t4 = lex.Next();       // let's see if an S is following
            lex.PushBack(t4);
			DEBUG("#7 In S, lookahead Token: %s", t4.ToString().c_str());
			if (t4.GetToken() != ")") {  // we *do* have a following S production
			    DEBUG("#8 In S"); 
				Snode.AddChild( S() );
			}
		}
	} 
	
	// ******************************  ==> atom || atom S
	else if (  firstToken.GetType() == Token::Type::Symbol 
			|| firstToken.GetType() == Token::Type::Number
			|| firstToken.GetType() == Token::Type::String) { 

		Snode.AddChild(SyntaxNode(firstToken, &Snode));

		Token lookAhead = lex.Next();   //checking if we're followed by an S
		DEBUG("#9 In S, Token: %s", lookAhead.ToString().c_str()); 
        lex.PushBack(lookAhead);

		if (lookAhead.GetToken() != ")") {   // then either a "(" or an atom
		    DEBUG("#10 In S"); 
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

