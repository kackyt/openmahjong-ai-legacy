// TakuEvent.h: CTakuEvent クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAKUEVENT_H__C2DDD854_58F3_4AB8_AEA9_1410A4E8CCFC__INCLUDED_)
#define AFX_TAKUEVENT_H__C2DDD854_58F3_4AB8_AEA9_1410A4E8CCFC__INCLUDED_

#include "Command.h"	// ClassView によって追加されました。
#include "Result.h"	// ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTakuEvent  
{
public:
	int m_iSeq;
	BOOL m_bActive;
	void parseXML(IXMLDOMNodePtr);
	CResult m_result;
	CCommand m_command;
	CTakuEvent();
	virtual ~CTakuEvent();

};

#endif // !defined(AFX_TAKUEVENT_H__C2DDD854_58F3_4AB8_AEA9_1410A4E8CCFC__INCLUDED_)
