// NakiMentsu.h: CNakiMentsu クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NAKIMENTSU_H__B8A10A5B_E396_4D25_A440_7B7209D08368__INCLUDED_)
#define AFX_NAKIMENTSU_H__B8A10A5B_E396_4D25_A440_7B7209D08368__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mentsu.h"
#include "Pai.h"	// ClassView によって追加されました。

class CNakiMentsu : public CMentsu  
{
public:
	UINT getNakiPos();
	CNakiMentsu& operator=(CNakiMentsu& value);
	virtual void parseXML(IXMLDOMNodePtr);
	CPai m_nakihai;
	int m_iAite;
	CNakiMentsu();
	virtual ~CNakiMentsu();

};

#endif // !defined(AFX_NAKIMENTSU_H__B8A10A5B_E396_4D25_A440_7B7209D08368__INCLUDED_)
