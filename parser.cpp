#include "lexer.h"
#include "syntax_tree.h"
#include "symbol_table.h"
#include "token.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

Lexer lex;

/** End Productions ***
 *   F-> <endoffile> || TF
 *   T-> (S)
 *	 S-> () || atom || (S) || ()S || atom S || (S)S
 */


class Parser {
	public:
		Parser();
		void F();
		SyntaxTree::Node T();
		SyntaxTree::Node S();

	private:
		SyntaxTree tree;
};

/*
Parser::expect(SyntaxTree::Node& t, char *expect_string)
{
	if (strcmp(t.GetToken(), expect_string) != 0){
		fprintf(stderr, "Expected one thing, got another\n");
		exit -1;
	}
}
*/
void Parser::F()
{
    while ( lex.HasNext() ) {
       tree.GetRoot().AddChild( T() ); //understood that this means "AddChild(T());"
    }
}

SyntaxTree::Node Parser::T() {return lex.Next();} //Hack to get it to compile

#if 0
SyntaxTree::Node T()
{
	SyntaxTree::Node Tnode;
	
	Token t = lex.Next(); //eat opening paren
	expect (t, "(");
	Tnode.AddChild( Node(t), &Tnode);
	Tnode.AddChild( S() , &Tnode);

	t = lex.Next(); //eat closing paren
	expect(t, ")");
	Tnode.AddChild( Node(t), &Tnode);

	return Tnode;
}

SyntaxTree::Node S()
{
	Token t = lex.Next();
	//****************** ==> () || (S) || ()S || (S)S 
	if (t.GetToken() == "(") {              
		AddChild(Node(t, this));  //adds the "(" to the list of children
		Token t2 = lex.Next();
	
		//****************** ==> () || ()S
		if (t2.GetToken() == ")") {      
			AddChild(Node(t2, this));  //add the ")" to the list of children
			Token t3 = lex.Next();
			if (t3.GetToken() == "(") {  //then we need to parse an S
				//somehow push back the token we just read
				AddChild( S() );
			}
		}
		//******************  ==> (S) || (S)S
		else { 
			//somehow push back the token we just read
			AddChild( S() );
			Token t4 = lex.Next();   //expect a ")"
			if (t4.GetToken() != ")"){
				fprintf(stderr, "Parse error: Expected a ')', but found %s\n", t4.GetToken());
				exit(-1);
			}
			AddChild( Node(t4, this) );  //add the ")" token
			Token t5 = lex.Next();       //let's see if an S is following
			if (t5.GetToken() == '(') {  // we think that an S  will follow
				AddChild( S() );
			}
		}
	} 
	/******************************  ==> atom || atom S */
	else if (t.GetType() == Symbol) { 
		AddChild(Node(t, this));
		Token t6 = lex.Next();   //checking if we're followed by an S
		if (t6.GetToken() == '(') {
			//somehow push back the current token
			AddChild( S() );
		}
	} 
	else {
		fprintf(stderr, "Expected '(' or an atom, but found '%s'\n", t.GetToken());
		exit(-1);
	}
}
#endif

/*
SyntaxTree& factor(Token token)
{
SyntaxTree t = lexer.Next(); 
while (lex.hasNext){
SyntaxTree op = GetToken()
op.addchild(t);
op.addchild(term());
t = op;
}
return t;
}

int main (int argc, char *argv[])
{
	if ((argc > 1) && strcmp(argv[1], "-help"))
	{
		fprintf(stderr, "Put your help message here\n");
		return 0;
    }
    // Node root = F();
	return 0;
    
}
*/
