// Event.h: CEvent クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENT_H__78F82895_0E38_44CB_ADDD_C93D9691D6B5__INCLUDED_)
#define AFX_EVENT_H__78F82895_0E38_44CB_ADDD_C93D9691D6B5__INCLUDED_

#include "Command.h"	// ClassView によって追加されました。
#include "Result.h"	// ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEvent  
{
public:
	void parseXML(IXMLDOMNodePtr);
	CResult m_result;
	CCommand m_command;
	CEvent();
	virtual ~CEvent();

};

#endif // !defined(AFX_EVENT_H__78F82895_0E38_44CB_ADDD_C93D9691D6B5__INCLUDED_)
