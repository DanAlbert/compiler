#include "symbol_table.h"
#include "token.h"
#include <assert.h>
#include "messages.h"

SymbolTable::SymbolTable(SymbolTable* parent) :
    parent(parent)
{
}
const Token& SymbolTable::operator[](const std::string& id) const
{
    return this->Get(id);
}

void SymbolTable::PrintSymbols(FILE* file) const
{
	assert(file);
	for ( auto it = this->symbols.begin(); it != this->symbols.end(); ++it ){
		it->second.Print(file);
		fprintf(file, "\n");
	}
}

inline const SymbolTable* SymbolTable::GetParent(void) const
{
	return this->parent;
}

void SymbolTable::Add(const Token& token)
{
    assert(!this->Contains(token.GetLexeme()));
    this->symbols.insert( {{token.GetLexeme(), token }});;
}

bool SymbolTable::Contains(const std::string& id) const
{
    if (this->symbols.find(id) != this->symbols.end())
    {
        return true;
    }
    else
    {
        if (this->parent != NULL)
        {
            return this->parent->Contains(id);
        }
        else
        {
            return false;
        }
    }
}

const Token& SymbolTable::Get(const std::string& id) const
{
    if (this->symbols.find(id) != this->symbols.end())
    {
        return this->symbols.find(id)->second;
    }
    else
    {
        if (this->parent != NULL)
        {
            return this->parent->Get(id);
        }
        else
        {
            CRITICAL("symbole undefined: %s", id.c_str());
            exit(EXIT_FAILURE);
        }
    }
}

