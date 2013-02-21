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
	SymbolTable(SymbolTable* parent = NULL);
	inline const SymbolTable* GetParent(void) const;
	const Token& operator[](const std::string& id) const;

	void Add(const Token& token);
	bool Contains(const std::string& id) const;
	const Token& Get(const std::string& id) const;

private:
	std::unordered_map<std::string, Token> symbols;
    SymbolTable* parent;
};

#endif // SYMBOL_TABLE_H

