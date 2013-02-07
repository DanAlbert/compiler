#include <string>
#include <gtest/gtest.h>
#include <symbol_table.h>

#include "random.h"

#define MAX_STRING_LENGTH 100
#define NRAND_TESTS 10000



TEST(SymbolTable, Add)
{
    std::vector<Token> key;
    std::vector<Token> value;
    SymbolTable table;
    for (int i = 0; i < NRAND_TESTS; i++)
    {
        Token token(Token::Type::Symbol, RandomString(MAX_STRING_LENGTH));
        table.Add(token);
        value.push_back(token);
        key.push_back(token.GetToken());
    }
    
    for (int i = 0; i < value.size()-1; i++)
    {
        Token current = value.back();
        value.pop_back();
        EXPECT_EQ(current, table[key.pop_back()]);
    }
}


TEST(SymbolTable, Get)
{
    for (int i = 0; i < NRAND_TESTS; i++)
    {
        std::vector<Token> tokens;
        SymbolTable table;
        for (int i = 0; i < 100; i++)
        {
            std::string value = RandomString(MAX_STRING_LENGTH);
            while (table.Contains(value))
            {
                value = RandomString(MAX_STRING_LENGTH);
            }
            
            Token token(Token::Type::Symbol, RandomString(MAX_STRING_LENGTH));
            table.Add(token);
            tokens.push_back(token);
        }
        
        for (int i = 0; i < tokens.size(); i++)
        {
            Token current = tokens.back();
            tokens.pop_back();
            EXPECT_TRUE(current == table.Get(current.GetToken()));
        }
    }
}


/*TEST(SymbolTable, Contains)
{
    for (int i = 0; i < arr.length; i++)
    {
         int x = RandomBetween(-500, 500);
         table.Add(x);
         arr[i] = x;
    }
    
}
*/



