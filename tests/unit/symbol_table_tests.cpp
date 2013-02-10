#include <string>
#include <gtest/gtest.h>
#include <symbol_table.h>

#include "random.h"

#define MAX_STRING_LENGTH 100
#define NRAND_TESTS 10000
#define NELEMS 100

TEST(SymbolTable, Add)
{
	for (int i = 0; i < NRAND_TESTS; i++)
	{
		std::vector<Token> tokens;
		SymbolTable table;
		for (int i = 0; i < NELEMS; i++)
		{
            std::string value = RandomString(MAX_STRING_LENGTH);
            while (table.Contains(value))
            {
                value = RandomString(MAX_STRING_LENGTH);
            }
            
            Token token(Token::Type::Symbol, value);
			table.Add(token);
			tokens.push_back(token);
		}
		
		for (auto it = tokens.begin(); it != tokens.end(); ++it)
		{
			EXPECT_EQ(*it, table[it->GetToken()]);
		}
	}
}

TEST(SymbolTable, Get)
{
    for (int i = 0; i < NRAND_TESTS; i++)
    {
        std::vector<Token> tokens;
        SymbolTable table;
        for (int i = 0; i < NELEMS; i++)
        {
            std::string value = RandomString(MAX_STRING_LENGTH);
            while (table.Contains(value))
            {
                value = RandomString(MAX_STRING_LENGTH);
            }
            
            Token token(Token::Type::Symbol, value);
            table.Add(token);
            tokens.push_back(token);
        }
        
    	for (auto it = tokens.begin(); it != tokens.end(); ++it)
		{
            EXPECT_EQ(*it, table.Get(it->GetToken()));
        }
    }
}

TEST(SymbolTable, Contains)
{
    for (int i = 0; i < NRAND_TESTS; i++)
    {
        std::vector<Token> tokens;
        SymbolTable table;
        for (int i = 0; i < NELEMS; i++)
        {
            std::string value = RandomString(MAX_STRING_LENGTH);
            while (table.Contains(value))
            {
                value = RandomString(MAX_STRING_LENGTH);
            }
            
            Token token(Token::Type::Symbol, value);
            table.Add(token);
            tokens.push_back(token);
        }
        
    	for (auto it = tokens.begin(); it != tokens.end(); ++it)
		{
            EXPECT_TRUE(table.Contains(it->GetToken()));
        }
    }
}

