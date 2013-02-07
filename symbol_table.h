/**
 * 
 * @author Dan Albert <dan@gingerhq.net>
 */
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <unordered_map>

#include "token.h"

class SymbolTable
{
public:
	SymbolTable(void);
	~SymbolTable(void);
	
	void Add(const Token& token);
	bool Contains(const std::string& id) const;
	Token& Get(const std::string& id);

private:
	std::unordered_map<std::string, Token> symbols;
};

#endif // SYMBOL_TABLE_H

