// Update 89 2.4.7 -> 97K - Filtro de palavras e nomes reconstruído
// Filter.h: interface for the CFilter class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <set>
#include <string>

class CFilter
{
public:
	CFilter();
	virtual ~CFilter();
	void Load(char* path);
	void CheckSyntax(char* text);
private:
	std::set<std::string> m_FilterInfo;
};

extern CFilter gFilter;
