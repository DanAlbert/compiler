#include "symbol_table.h"

#include <assert.h>

void SymbolTable::Add(const Token& token)
{
	this->symbols[token.GetToken()] = token;
}

bool SymbolTable::Contains(const std::string& id) const
{
	return this->symbols.find(id) != this->symbols.end();
}

Token& SymbolTable::Get(const std::string& id)
{
	assert(this->Contains(id));
	return this->symbols[id];
}

