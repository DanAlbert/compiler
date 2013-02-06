#include <string>
#include <gtest/gtest.h>

#include <token.h>

#include "random.h"

#define MAX_STRING_LENGTH 100
#define NRAND_TESTS 10000

Token::Type RandomType(void);

TEST(Token, Constructor)
{
	for (int i = 0; i < NRAND_TESTS; i++)
	{
		Token::Type type = RandomType();
		std::string str = RandomString(MAX_STRING_LENGTH);

		Token token(type, str);

		EXPECT_EQ(type, token.GetType());
		EXPECT_EQ(str, token.GetToken());
	}
}

TEST(Token, ToString)
{
	for (int i = 0; i < NRAND_TESTS; i++)
	{
		Token::Type type = RandomType();
		std::string str = RandomString(MAX_STRING_LENGTH);

		Token token(type, str);
		
		std::ostringstream builder;
		builder << '<' << TokenTypeToString(token.GetType()) << ", "
			    << token.GetToken() << '>';
		std::string expected = builder.str();

		EXPECT_EQ(expected, token.ToString());
	}
}

Token::Type RandomType(void)
{
	int idx = RandomBetween(0, static_cast<int>(Token::Type::Symbol)); 
	return static_cast<Token::Type>(idx);
}

