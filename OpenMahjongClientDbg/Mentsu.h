// Mentsu.h: CMentsu クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENTSU_H__36705156_5BAB_41C5_B25A_462B1E908550__INCLUDED_)
#define AFX_MENTSU_H__36705156_5BAB_41C5_B25A_462B1E908550__INCLUDED_

#include "Pai.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMentsu  
{
public:
	CMentsu& operator=(CMentsu& value);
	virtual void parseXML(IXMLDOMNodePtr);
	int m_iCategory;
	CArray<CPai,CPai&> m_aPaiList;
	CMentsu();
	virtual ~CMentsu();

};

#endif // !defined(AFX_MENTSU_H__36705156_5BAB_41C5_B25A_462B1E908550__INCLUDED_)
