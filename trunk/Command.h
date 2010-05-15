// Command.h: CCommand クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMAND_H__AF678D29_3677_4E6C_9CF9_14B3565CAFCE__INCLUDED_)
#define AFX_COMMAND_H__AF678D29_3677_4E6C_9CF9_14B3565CAFCE__INCLUDED_

#include "Player.h"	// ClassView によって追加されました。
#include "Pai.h"	// ClassView によって追加されました。
#include "NakiMentsu.h"	// ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCommand  
{
public:
	void toXML(IXMLDOMDocumentPtr,IXMLDOMElementPtr);
	CCommand& operator=(CCommand&);
	CNakiMentsu m_mentsu;
	void parseXML(IXMLDOMNodePtr);
	CPai m_pai;
	CPlayer m_player;
	int m_iType;
	int m_iId;
	CCommand();
	virtual ~CCommand();

};

#endif // !defined(AFX_COMMAND_H__AF678D29_3677_4E6C_9CF9_14B3565CAFCE__INCLUDED_)
