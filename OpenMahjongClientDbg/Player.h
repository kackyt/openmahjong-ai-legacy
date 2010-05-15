// Player.h: CPlayer クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYER_H__521B1B37_B5BC_4B85_A9EC_F41883A29BE7__INCLUDED_)
#define AFX_PLAYER_H__521B1B37_B5BC_4B85_A9EC_F41883A29BE7__INCLUDED_

#include "MIPIface.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPlayer  
{
public:
	void toXML(IXMLDOMDocumentPtr,IXMLDOMElementPtr);
	void parseXML(IXMLDOMNodePtr);
	MJPIFunc m_pFunc;
	LPVOID m_pInst;
	BOOL m_bIsComp;
	CString m_strName;
	int m_iId;
	CPlayer();
	virtual ~CPlayer();

};

#endif // !defined(AFX_PLAYER_H__521B1B37_B5BC_4B85_A9EC_F41883A29BE7__INCLUDED_)
