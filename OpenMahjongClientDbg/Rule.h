// Rule.h: CRule クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RULE_H__1DF0E466_F181_4151_8F1A_F6CF271C987F__INCLUDED_)
#define AFX_RULE_H__1DF0E466_F181_4151_8F1A_F6CF271C987F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRule  
{
public:
	BOOL m_bActive;
	void toXML(IXMLDOMDocumentPtr,IXMLDOMElementPtr);
	void parseXML(IXMLDOMNodePtr);
	int m_iKuitan;
	int m_iGlasshai;
	CRule();
	virtual ~CRule();

};

#endif // !defined(AFX_RULE_H__1DF0E466_F181_4151_8F1A_F6CF271C987F__INCLUDED_)
