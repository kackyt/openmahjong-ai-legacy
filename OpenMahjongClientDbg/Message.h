// Message.h: CMessage クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGE_H__D2B10CD7_C23F_4808_BF38_883241DBC8B5__INCLUDED_)
#define AFX_MESSAGE_H__D2B10CD7_C23F_4808_BF38_883241DBC8B5__INCLUDED_

#include "Player.h"	// ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMessage  
{
public:
	CMessage& operator=(CMessage&);
	void toXML(IXMLDOMDocumentPtr,IXMLDOMElementPtr);
	void parseXML(IXMLDOMNodePtr);
	CArray<CPlayer,CPlayer&> m_aPlayerTo;
	CString m_strText;
	CPlayer m_playerFrom;
	CMessage();
	virtual ~CMessage();

};

#endif // !defined(AFX_MESSAGE_H__D2B10CD7_C23F_4808_BF38_883241DBC8B5__INCLUDED_)
