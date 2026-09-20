// BadSyntax.h: interface for the CBadSyntax class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CBadSyntax
{
public:
	CBadSyntax();
	virtual ~CBadSyntax();
	void Load(char* path);
	bool CheckSyntax(char* text);
private:
	std::vector<std::string> m_BadSyntaxInfo;
};

extern CBadSyntax gBadSyntax;
