#include "lexer.h"
#include "syntax_tree.h"
#include "symbol_table.h"
#include "token.h"
#include "messages.h"
#include "parser.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>


/*** End Productions ***
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

Parser::~Parser(void)
{
	if (this->root)
	{
		delete this->root;
		this->root = NULL;
	}
}

void Parser::ParseTree(void)
{
    this->root = new SyntaxNode(Token(Token::Type::Symbol, "statements"));
	this->F(this->root);
}

void Parser::PrintTree(FILE* file)
{
	assert(this->root);
    this->root->Print(file);
}

void Parser::expect(Token t, const char *expect_string)
{
	if (t.GetLexeme() != expect_string) {
		ERROR("Expecting string '%s' but encountered '%s'",
		      expect_string, t.GetLexeme().c_str());
		exit(EXIT_FAILURE);
	}
}

void Parser::F(SyntaxNode* parent)
{
	assert(parent);

	DEBUG("In F");

	while (lex.HasNext())
	{
		DEBUG("In F, about to call T"); 
		this->T(parent);
	}
}

void Parser::T(SyntaxNode* parent)
{
	assert(parent);

	Token t = lex.Next(); //eat opening paren
	DEBUG("In T, Token: %s", t.ToString().c_str());
	expect(t, "(");
	SyntaxNode* T = parent->AddChild(t);
	
	//Add to symbol table
	if(!symtab.Contains(t.GetLexeme())){
		symtab.Add(t);	
	}

	this->S(T);

	t = lex.Next(); //eat closing paren
	DEBUG("In T, Token: %s", t.ToString().c_str());
	expect(t, ")");
	parent->AddChild(t);

	//Add to symbol table
	if(!symtab.Contains(t.GetLexeme())){
		symtab.Add(t);	
	}
}

void Parser::S(SyntaxNode* parent)
{
	assert(parent);

	Token firstToken = lex.Next();
	DEBUG("#1 In S, Token: %s", firstToken.ToString().c_str());
	// ****************** ==> () || (S) || ()S || (S)S 
	if (firstToken.GetLexeme() == "(") {
		SyntaxNode* S = parent->AddChild(firstToken);  //adds the "(" to the list of children
		//Add to symbol table
		if(!symtab.Contains(firstToken.GetLexeme())){
			symtab.Add(firstToken);	
		}

		Token t = lex.Next();
		DEBUG("#2 In S, Token: %s", t.ToString().c_str()); 
		// ****************** ==> () || ()S
		if (t.GetLexeme() == ")") {      
			parent->AddChild(t);  //add the ")" to the list of children
			
			//Add to symbol table
			if(!symtab.Contains(t.GetLexeme())){
				symtab.Add(t);	
			}

            // We now distinguish between () and ()S
			Token t2 = lex.Next();
            lex.PushBack(t2);
			DEBUG("#3 In S --> () | ()S, Token: %s", t2.ToString().c_str()); 
			if (t2.GetLexeme() == ")") {  // this is the () case
			    DEBUG("#4 In S --> (), Token: %s", t2.ToString().c_str()); 
			}
            else {                       // this is the ()S case
			    DEBUG("#5 In S --> ()S, Token: %s", t2.ToString().c_str());
				this->S(parent);
			}
		}
		// ******************  ==> (S) || (S)S
		else { 
            lex.PushBack(t);  // this token belongs to the S production
			this->S(S);

			Token t3 = lex.Next();   //expect a ")"
			DEBUG("#6 In S -> (S) || (S)S, Token: %s", t3.ToString().c_str()); 
            expect(t3, ")");
			parent->AddChild(t3);  //add the ")" token

			//Add to symbol table
			if(!symtab.Contains(t3.GetLexeme())){
				symtab.Add(t3);	
			}

			Token t4 = lex.Next();       // let's see if an S is following
            lex.PushBack(t4);
			DEBUG("#7 In S, lookahead Token: %s", t4.ToString().c_str());
			if (t4.GetLexeme() != ")") {  // we *do* have a following S production
			    DEBUG("#8 In S"); 
				this->S(parent);
			}
		}
	} 
	
	// ******************************  ==> atom || atom S
	else if (  firstToken.GetType() == Token::Type::Symbol 
			|| firstToken.GetType() == Token::Type::Number
			|| firstToken.GetType() == Token::Type::String) { 

		parent->AddChild(firstToken);
		
		//1 liner version maybe: symtab[firstToken.GetLexeme()] = firstToken();
		//Add to symbol table
		if(!symtab.Contains(firstToken.GetLexeme())){
			symtab.Add(firstToken);	
		}

		Token lookAhead = lex.Next();   //checking if we're followed by an S
		DEBUG("#9 In S, Token: %s", lookAhead.ToString().c_str()); 
        lex.PushBack(lookAhead);

		if (lookAhead.GetLexeme() != ")") {   // then either a "(" or an atom
		    DEBUG("#10 In S"); 
			this->S(parent);
		}
	} 

    // *****************************  Doesn't match any production
	else {
		fprintf(stderr, "Expected '(' or a symbol, but found '%s'\n",
            firstToken.GetLexeme().c_str());
		exit(-1);
	}
}

