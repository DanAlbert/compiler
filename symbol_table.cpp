#include "symbol_table.h"

#include <assert.h>

SymbolTable::SymbolTable(void)
{
}

const Token& SymbolTable::operator[](const std::string& id) const
{
	return this->Get(id);
}

void SymbolTable::Add(const Token& token)
{
	assert(!this->Contains(token.GetToken()));
	this->symbols.insert( {{token.GetToken(), token }});;
}

bool SymbolTable::Contains(const std::string& id) const
{
	return this->symbols.find(id) != this->symbols.end();
}

const Token& SymbolTable::Get(const std::string& id) const
{
	assert(this->Contains(id));
	return this->symbols.find(id)->second;
}

